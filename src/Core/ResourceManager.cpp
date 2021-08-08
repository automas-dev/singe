#include "s3e/Core/ResourceManager.hpp"

#include <filesystem>

namespace fs = std::filesystem;

#include <array>
#include <glm/glm.hpp>
#include <map>
#include <vector>

#include "s3e/Support/Parser.hpp"
#include "s3e/Support/Util.hpp"
#include "s3e/Support/log.hpp"

#define PARSE_ERROR(TAG)                                                    \
    {                                                                       \
        Logging::Core->error("Failed to parse tag {}: {}", (TAG), line_no); \
        return nullptr;                                                     \
    }

#define ASSERT_NREAD(N, TAG) \
    if (nRead != (N))        \
        PARSE_ERROR(TAG);

namespace Tom::s3e {
    ResourceManagerBase::ResourceManagerBase() : ResourceManagerBase("./") {}

    ResourceManagerBase::ResourceManagerBase(const std::string & path)
        : rootPath(path) {}

    ResourceManagerBase::~ResourceManagerBase() {}

    void ResourceManagerBase::setResourcePath(const std::string & path) {
        rootPath = path;
    }

    const std::string & ResourceManagerBase::getResourcePath() {
        return rootPath;
    }

    std::string ResourceManagerBase::resourceAt(const std::string & path) const {
        fs::path base(path);
        if (base.is_absolute())
            return path;
        else
            return {fs::path(rootPath) / base};
    }
}

namespace Tom::s3e {
    Texture::Ptr ResourceManager::loadTexture(const std::string & path,
                                              GLint magFilter,
                                              GLint minFilter) {
        if (textures.count(path) > 0)
            return textures[path];

        auto relPath = resourceAt(path);
        sf::Image image;
        if (!image.loadFromFile(relPath)) {
            Logging::Core->error("failed in call to Texture::loadFromPath(path={})",
                                 relPath);
            return nullptr;
        }

        auto newTex = std::make_shared<Texture>(image, magFilter, minFilter);
        textures[path] = newTex;
        return newTex;
    }

    Shader::Ptr ResourceManager::loadShader(const std::string & vertexPath,
                                            const std::string & fragmentPath) {
        auto relVertexPath = resourceAt(vertexPath);
        auto relFragmentPath = resourceAt(fragmentPath);
        auto shader = std::make_shared<Shader>();
        if (!shader->loadFromPath(relVertexPath, relFragmentPath)) {
            Logging::Core->error(
                "failed in call to Shader::loadFromPath(vertexPath={}, fragmentPath={})",
                relVertexPath, relFragmentPath);
            return nullptr;
        }
        return shader;
    }

    std::unique_ptr<MaterialLib> ResourceManager::loadMaterials(
        const std::string & objPath, const std::string & name) {
        auto mtllib = std::make_unique<MaterialLib>();
        mtllib->name = name;

        std::string mtlPath = objPath.substr(0, objPath.find_last_of('/') + 1);
        mtlPath += name;
        mtlPath = resourceAt(mtlPath);

        WavefrontParser parser;
        parser.open(mtlPath);

        if (!parser.is_open()) {
            Logging::Core->error("Failed to open material library {}", mtlPath);
            return nullptr;
        }

        Material::Ptr material;

        for (auto & token : parser.tokens()) {
            switch (token.key[0]) {
                case 'n':
                    if (token.key == "newmtl") {
                        if (material)
                            mtllib->materials[material->name] = material;
                        material = std::make_shared<Material>();
                        material->name = token.value;
                    }
                    break;
                case 'K':
                    switch (token.key[1]) {
                        case 'a': // Ka
                        {
                            auto parts = token.params();
                            if (parts.size() != 3)
                                return nullptr;
                            material->ambient.x = std::stof(parts[0]);
                            material->ambient.y = std::stof(parts[1]);
                            material->ambient.z = std::stof(parts[2]);
                        } break;
                        case 'd': // Kd
                        {
                            auto parts = token.params();
                            if (parts.size() != 3)
                                return nullptr;
                            material->diffuse.x = std::stof(parts[0]);
                            material->diffuse.y = std::stof(parts[1]);
                            material->diffuse.z = std::stof(parts[2]);
                        } break;
                        case 's': // Ks
                        {
                            auto parts = token.params();
                            if (parts.size() != 3)
                                return nullptr;
                            material->specular.x = std::stof(parts[0]);
                            material->specular.y = std::stof(parts[1]);
                            material->specular.z = std::stof(parts[2]);
                        } break;
                        default:
                            break;
                    }
                    break;
                case 'N':
                    switch (token.key[1]) {
                        case 'i': // Ni
                            material->specExp = std::stof(token.value);
                            break;
                        default:
                            break;
                    }
                    break;
                case 'd':
                    material->alpha = std::stof(token.value);
                    break;
                case 'm':
                    if (token.key == "map_Kd") {
                        material->texture = loadTexture(token.value);
                    }
                    break;
                default:
                    break;
            }
        }

        if (material) {
            mtllib->materials[material->name] = material;
        }

        return mtllib;
    }

    struct TempModel {
        struct Face {
            struct Point {
                std::size_t v, t, n;
            };
            std::array<Point, 3> p;
        };

        std::string name;
        Material::Ptr material;
        std::vector<Face> af;

        TempModel(const std::string & name) : name(name) {}

        Model::Ptr toModel(const std::vector<glm::vec3> & av,
                           const std::vector<glm::vec2> & avt,
                           const std::vector<glm::vec3> & avn) {
            auto model = std::make_shared<Model>(name);
            model->materials.push_back(material);
            for (auto & face : af) {
                for (auto & point : face.p) {
                    if (point.v > av.size() || point.n > avn.size()
                        || point.t > avt.size()) {
                        Logging::Core->error("Point out of range");
                    }
                    model->points.emplace_back(av[point.v - 1],
                                               avn[point.n - 1],
                                               avt[point.t - 1]);
                }
            }
            return model;
        }
    };


    Scene::Ptr ResourceManager::loadScene(const std::string & path) {
        auto relPath = resourceAt(path);

        WavefrontParser parser;
        parser.open(relPath);

        if (!parser.is_open()) {
            Logging::Core->warning("Failed to open file {}", relPath);
            return nullptr;
        }

        std::string sceneName;
        {
            auto i = relPath.find_last_of('/');
            if (i != std::string::npos) {
                i++;
                auto n = relPath.size() - 4 - i;
                sceneName = relPath.substr(i, n);
            }
        }

        auto scene = std::make_shared<Scene>(sceneName);
        std::unique_ptr<TempModel> tmpModel;
        std::unique_ptr<MaterialLib> mtllib;

        std::vector<glm::vec3> av;
        std::vector<glm::vec2> avt;
        std::vector<glm::vec3> avn;

        int nRead;
        std::string line;
        std::string str;
        for (auto & token : parser.tokens()) {
            switch (token.key[0]) {
                case 'o':
                    if (tmpModel)
                        scene->models.push_back(tmpModel->toModel(av, avt, avn));
                    tmpModel = std::make_unique<TempModel>(token.value);
                    break;
                case 'v':
                    if (token.key.size() == 1) // v
                    {
                        auto parts = token.params();
                        if (parts.size() != 3)
                            return nullptr;
                        glm::vec3 v;
                        v.x = std::stof(parts[0]);
                        v.y = std::stof(parts[1]);
                        v.z = std::stof(parts[2]);
                        av.push_back(v);
                    }
                    else {
                        switch (token.key[1]) {
                            case 't': // vt
                            {
                                auto parts = token.params();
                                if (parts.size() != 2)
                                    return nullptr;
                                glm::vec2 vt;
                                vt.x = std::stof(parts[0]);
                                vt.y = std::stof(parts[1]);
                                avt.push_back(vt);
                            } break;
                            case 'n': // vn
                            {
                                auto parts = token.params();
                                if (parts.size() != 3)
                                    return nullptr;
                                glm::vec3 vn;
                                vn.x = std::stof(parts[0]);
                                vn.y = std::stof(parts[1]);
                                vn.z = std::stof(parts[2]);
                                avn.push_back(vn);
                            } break;
                        }
                    }
                    break;
                case 'f': {
                    auto parts = token.params();
                    if (parts.size() != 3)
                        return nullptr;
                    TempModel::Face f;
                    for (int i = 0; i < 3; i++) {
                        auto subparts = splitString(parts[i], '/');
                        if (subparts.size() != 3)
                            return nullptr;
                        f.p[i].v = std::stoul(subparts[0]);
                        f.p[i].t = std::stoul(subparts[1]);
                        f.p[i].n = std::stoul(subparts[2]);
                    }
                    tmpModel->af.push_back(f);
                } break;
                case 'm': // mtllib
                    if (token.key != "mtllib")
                        return nullptr;
                    mtllib = loadMaterials(path, token.value);
                    break;
                case 'u': // usemtl
                    if (token.key != "usemtl")
                        return nullptr;
                    tmpModel->material = mtllib->materials[token.value];
                    break;
                default:
                    break;
            }
        }

        if (tmpModel)
            scene->models.push_back(tmpModel->toModel(av, avt, avn));

        return scene;
    }
}

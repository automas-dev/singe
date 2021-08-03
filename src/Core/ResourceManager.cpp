#include "s3e/Core/ResourceManager.hpp"

#include <filesystem>
namespace fs = std::filesystem;

#include <array>
#include <cstring>
#include <fstream>
#include <glm/glm.hpp>
#include <map>
#include <vector>

#include "s3e/Support/Parser.hpp"
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
    Texture::Ptr ResourceManager::loadTexture(const std::string & path) {
        if (textures.count(path) > 0)
            return textures[path];

        auto relPath = resourceAt(path);
        auto newTex = std::make_shared<Texture>();
        if (!newTex->loadFromPath(relPath)) {
            Logging::Core->error("failed in call to Texture::loadFromPath(path={})",
                                 relPath);
            return nullptr;
        }
        else
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
            model->send();
            return model;
        }
    };

    Scene::Ptr ResourceManager::loadScene(const std::string & path) {
        auto relPath = resourceAt(path);
        std::ifstream is(relPath);

        if (!is.is_open()) {
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
        for (std::size_t line_no = 0; std::getline(is, line); line_no++) {
            if (line.empty() || line[0] == '#')
                continue;

            switch (line[0]) {
                case 'o':
                    if (tmpModel)
                        scene->models.push_back(tmpModel->toModel(av, avt, avn));
                    tmpModel = std::make_unique<TempModel>(line.substr(2));
                    break;
                case 'v':
                    switch (line[1]) {
                        case ' ': // v
                            glm::vec3 v;
                            str = line.substr(2);
                            nRead = std::sscanf(str.c_str(), "%f %f %f", &v.x,
                                                &v.y, &v.z);
                            ASSERT_NREAD(3, 'v');
                            av.push_back(v);
                            break;
                        case 't': // vt
                            glm::vec2 vt;
                            str = line.substr(3);
                            nRead =
                                std::sscanf(str.c_str(), "%f %f", &vt.x, &vt.y);
                            ASSERT_NREAD(2, "vt");
                            avt.push_back(vt);
                            break;
                        case 'n': // vn
                            glm::vec3 vn;
                            str = line.substr(3);
                            nRead = std::sscanf(str.c_str(), "%f %f %f", &vn.x,
                                                &vn.y, &vn.z);
                            ASSERT_NREAD(3, "vn");
                            avn.push_back(vn);
                            break;
                    }
                    break;
                case 'f':
                    TempModel::Face f;
                    str = line.substr(2);
                    nRead = std::sscanf(
                        str.c_str(), "%lu/%lu/%lu %lu/%lu/%lu %lu/%lu/%lu",
                        &f.p[0].v, &f.p[0].t, &f.p[0].n, &f.p[1].v, &f.p[1].t,
                        &f.p[1].n, &f.p[2].v, &f.p[2].t, &f.p[2].n);
                    tmpModel->af.push_back(f);
                    break;
                case 'm': // mtllib
                    if (line.size() < 8)
                        PARSE_ERROR("mtllib")
                    mtllib = loadMaterials(path, line.substr(7));
                    break;
                case 'u': // usemtl
                    if (line.size() < 8)
                        PARSE_ERROR("usemtl")
                    tmpModel->material = mtllib->materials[line.substr(7)];
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

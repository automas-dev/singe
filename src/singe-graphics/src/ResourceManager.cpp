#include "singe/Graphics/ResourceManager.hpp"

#include <filesystem>

namespace fs = std::filesystem;

#include <array>
#include <fstream>
#include <glm/glm.hpp>
#include <map>
#include <vector>

#include "Wavefront.hpp"
#include "singe/Support/Util.hpp"
#include "singe/Support/log.hpp"

#define PARSE_ERROR(TAG)                                                    \
    {                                                                       \
        Logging::Core->error("Failed to parse tag {}: {}", (TAG), line_no); \
        return nullptr;                                                     \
    }

#define ASSERT_NREAD(N, TAG) \
    if (nRead != (N))        \
        PARSE_ERROR(TAG);

namespace singe {
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

namespace singe {
    std::shared_ptr<Texture> ResourceManager::loadTexture(const std::string & path,
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

        auto newTex = std::make_shared<Texture>(image.getPixelsPtr(),
                                                glm::uvec2(image.getSize().x, image.getSize().y),
                                                4);
        textures[path] = newTex;
        return newTex;
    }

    std::shared_ptr<Shader> ResourceManager::loadShader(
        const std::string & vertexPath, const std::string & fragmentPath) {
        auto relVertexPath = resourceAt(vertexPath);
        auto relFragmentPath = resourceAt(fragmentPath);
        auto shader = Shader::fromPaths(relVertexPath, relFragmentPath);
        return std::make_shared<Shader>(std::move(shader));
    }

    std::shared_ptr<Shader> ResourceManager::loadShader(const std::string & fragmentPath) {
        auto relFragmentPath = resourceAt(fragmentPath);
        auto shader = Shader::fromFragmentPath(relFragmentPath);
        return std::make_shared<Shader>(std::move(shader));
    }

    Scene::Ptr ResourceManager::loadScene(const std::string & path) {
        auto relPath = resourceAt(path);

        std::string sceneName;
        {
            auto i = relPath.find_last_of('/');
            if (i != std::string::npos) {
                i++;
                auto n = relPath.size() - 4 - i;
                sceneName = relPath.substr(i, n);
            }
        }

        wavefront::Model model;
        try {
            model.loadModelFrom(relPath);
        }
        catch (const wavefront::ModelLoadException & e) {
            throw std::runtime_error(e.what());
        }

        auto scene = std::make_shared<Scene>(sceneName);
        for (auto & obj : model.objects) {
            auto mdl = std::make_shared<Model>(obj->name);
            scene->models.push_back(mdl);

            auto mesh = std::make_shared<Mesh>();
            for (int i = 0; i < obj->size(); i++) {
                mesh->buffer.buff.emplace_back(obj->vertices[i],
                                               obj->normals[i],
                                               obj->texcoords[i]);
            }
            mdl->geometry.emplace_back(mesh, 0);
            auto & mat = model.materials[obj->matId];
            auto material = std::make_shared<Material>();
            mdl->materials.push_back(material);

            material->name = mat->name;
            material->ambient = mat->colAmbient;
            material->diffuse = mat->colDiffuse;
            material->specular = mat->colSpecular;
            material->specExp = mat->specExp;
            material->alpha = mat->alpha;

            if (!mat->texAlbedo.empty())
                material->texture = loadTexture(mat->texAlbedo);
            if (!mat->texNormal.empty())
                material->normalTexture = loadTexture(mat->texNormal);
            if (!mat->texSpecular.empty())
                material->specularTexture = loadTexture(mat->texSpecular);
        }

        return scene;
    }
}

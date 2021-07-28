#include "s3e/Core/ResourceManager.hpp"

#include <filesystem>

#include "s3e/Support/log.hpp"
namespace fs = std::filesystem;

namespace Tom::s3e {
    ResourceManager::ResourceManager() : ResourceManager("./") {}

    ResourceManager::ResourceManager(const std::string & path)
        : rootPath(path) {}

    ResourceManager::~ResourceManager() {}

    void ResourceManager::setResourcePath(const std::string & path) {
        rootPath = path;
    }

    const std::string & ResourceManager::getResourcePath() {
        return rootPath;
    }

    std::string ResourceManager::resourceAt(const std::string & path) const {
        fs::path base(path);
        if (base.is_absolute())
            return path;
        else
            return {fs::path(rootPath) / base};
    }
}

namespace Tom::s3e {
    Texture::Ptr DefaultResourceManager::loadTexture(const std::string & name) {
        if (textures.count(name) > 0)
            return textures[name];
        else
            return nullptr;
    }

    Texture::Ptr DefaultResourceManager::loadTexture(const std::string & name,
                                                     const std::string & path) {
        auto relPath = resourceAt(path);
        auto newTex = std::make_shared<Texture>();
        if (!newTex->loadFromPath(relPath)) {
            SPDLOG_ERROR("failed in call to Texture::loadFromPath(path={})",
                         relPath);
            return nullptr;
        }
        else
            textures[name] = newTex;
        return newTex;
    }

    Shader::Ptr DefaultResourceManager::loadShader(const std::string & vertexPath,
                                                   const std::string & fragmentPath) {
        auto relVertexPath = resourceAt(vertexPath);
        auto relFragmentPath = resourceAt(fragmentPath);
        auto shader = std::make_shared<Shader>();
        if (!shader->loadFromPath(relVertexPath, relFragmentPath)) {
            SPDLOG_ERROR(
                "failed in call to Shader::loadFromPath(vertexPath={}, fragmentPath={})",
                relVertexPath, relFragmentPath);
            return nullptr;
        }
        return shader;
    }

    Model::Ptr DefaultResourceManager::loadModel(const std::string & path) {
        auto relPath = resourceAt(path);
        auto model = std::make_shared<Model>();
        if (!model->loadFromPath(relPath)) {
            SPDLOG_ERROR("failed in call to Model::loadFromPath(path={})", relPath);
            return nullptr;
        }
        return model;
    }
}

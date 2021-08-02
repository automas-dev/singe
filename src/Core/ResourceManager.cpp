#include "s3e/Core/ResourceManager.hpp"

#include <filesystem>
namespace fs = std::filesystem;

#include "s3e/Core/ModelLoader.hpp"
#include "s3e/Support/log.hpp"

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

    Model::Ptr ResourceManager::loadModel(const std::string & path) {
        auto relPath = resourceAt(path);
        ModelLoader loader;
        auto model = loader.loadModel(relPath);
        if (!model) {
            Logging::Core->error(
                "failed in call to ModelLoader::loadModel(path={})", relPath);
            return nullptr;
        }
        return model;
    }
}

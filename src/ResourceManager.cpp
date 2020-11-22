#include "s3e/ResourceManager.hpp"
#include <filesystem>
namespace fs = std::filesystem;

namespace Tom::s3e {
    ResourceManager::ResourceManager() : ResourceManager("./") { }

    ResourceManager::ResourceManager(const std::string & path) : rootPath(path) { }

    ResourceManager::~ResourceManager() { }

    void ResourceManager::setResourcePath(const std::string & path) {
        rootPath = path;
    }

    const std::string &ResourceManager::getResourcePath() {
        return rootPath;
    }

    std::string ResourceManager::resPath(const std::string & path) const {
        auto resProtoPos = path.find("res://");

        if (resProtoPos ==  0)
            return { fs::path(rootPath) / fs::path(path.substr(6)) };
        else
            return path;
    }
}

namespace Tom::s3e {
    Texture::Ptr DefaultResourceManager::loadTexture(const std::string & name) {
        if (textures.count(name) > 0)
            return textures[name];
        else
            return nullptr;
    }

    Texture::Ptr DefaultResourceManager::loadTexture(const std::string & name, const std::string & path) {
        auto newTex = Texture::create(resPath(path));
        if (newTex)
            textures[name] = newTex;
        return newTex;
    }

    Shader::Ptr DefaultResourceManager::loadShader(const std::string & vertexPath, const std::string & fragmentPath) {
        return Shader::create(resPath(vertexPath), resPath(fragmentPath));
    }

    Model::Ptr DefaultResourceManager::loadModel(const std::string & path) {
        return Model::create(resPath(path));
    }
}


#include "s3e/ResourceManager.hpp"
#include "s3e/log.hpp"
#include <filesystem>
namespace fs = std::filesystem;

namespace Tom::s3e {
    ResourceManager::ResourceManager() : ResourceManager("./") { }

    ResourceManager::ResourceManager(const std::string & path) : rootPath(path) { }

    ResourceManager::~ResourceManager() { }

    void ResourceManager::setResourcePath(const std::string & path) {
        rootPath = path;
    }

    const std::string & ResourceManager::getResourcePath() {
        return rootPath;
    }

    std::string ResourceManager::resourceAt(const std::string & path) const {
        auto resProtoPos = path.find("res://");

        if (resProtoPos ==  0)
            return { fs::path(rootPath) / fs::path(path.substr(6)) };
        else
            return path;
    }

    ResourceManager::Ptr ResourceManager::create(const std::string & path) {
        auto resManager = std::make_shared<ResourceManager>(path);
        return resManager;
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
        auto relPath = resourceAt(path);
        auto newTex = Texture::create(relPath);
        if (!newTex)
            SPDLOG_ERROR("failed in call to Texture::create(path={})", relPath);
        else
            textures[name] = newTex;
        return newTex;
    }

    Shader::Ptr DefaultResourceManager::loadShader(const std::string & vertexPath, const std::string & fragmentPath) {
        auto relVertexPath = resourceAt(vertexPath);
        auto relFragmentPath = resourceAt(fragmentPath);
        auto shader = Shader::create(relVertexPath, relFragmentPath);
        if (!shader)
            SPDLOG_ERROR("failed in call to Shader::create(vertexPath={}, fragmentPath={})", relVertexPath, relFragmentPath);
        return shader;
    }

    Model::Ptr DefaultResourceManager::loadModel(const std::string & path) {
        auto relPath = resourceAt(path);
        auto model = Model::create(relPath);
        if (!model)
            SPDLOG_ERROR("failed in call to Model::create(path={})", relPath);
        return model;
    }

    DefaultResourceManager::Ptr DefaultResourceManager::create(const std::string & path) {
        auto resManager = std::make_shared<DefaultResourceManager>(path);
        return resManager;
    }
}


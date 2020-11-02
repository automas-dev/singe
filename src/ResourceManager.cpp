#include "ResourceManager.hpp"
#include <filesystem>
namespace fs = std::filesystem;

namespace game {
    ResourceManager::ResourceManager() : ResourceManager("") { }

    ResourceManager::ResourceManager(const sf::String & path) : resPath(path) { }

    ResourceManager::~ResourceManager() { }

    void ResourceManager::setResourcePath(const sf::String & path) {
        resPath = path;
    }

    Texture::Ptr ResourceManager::loadTexture(const sf::String & name) {
        if (textures.count(name) > 0)
            return textures[name];
        else
            return nullptr;
    }

    Texture::Ptr ResourceManager::loadTexture(const sf::String & name, const sf::String & path) {
        auto newTex = Texture::create(parsePath(path));
        if (newTex)
            textures[name] = newTex;
        return newTex;
    }

    Shader::Ptr ResourceManager::loadShader(const std::string & vertexPath, const std::string & fragmentPath) {
        return Shader::create(parsePath(vertexPath), parsePath(fragmentPath));
    }

    Model::Ptr ResourceManager::loadModel(const std::string & path) {
        return Model::create(parsePath(path));
    }

    sf::String ResourceManager::parsePath(const sf::String & path) const {
        auto resProtoPos = path.find("res://");

        if (resProtoPos ==  0)
            return { fs::path(resPath) / fs::path(path.substring(6)) };
        else
            return path;
    }
}
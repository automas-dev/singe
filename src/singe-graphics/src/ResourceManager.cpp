#include "singe/Graphics/ResourceManager.hpp"

#include <Wavefront.hpp>
#include <filesystem>

namespace fs = std::filesystem;

namespace singe {
    using std::make_unique;
    using std::move;

    ResourceManager::ResourceManager(const path & root) : root(root) {}

    ResourceManager::ResourceManager(ResourceManager && other)
        : root(other.root) {}

    ResourceManager & ResourceManager::operator=(ResourceManager && other) {
        root = other.root;
        return *this;
    }

    ResourceManager::~ResourceManager() {}

    void ResourceManager::setRoot(const path & root) {
        this->root = root;
    }

    const path & ResourceManager::getRoot() const {
        return root;
    }

    path ResourceManager::resourceAt(const path & subPath) const {
        if (subPath.is_absolute())
            return subPath;
        else
            return root / subPath;
    }

    Texture & ResourceManager::getTexture(const string & name) {
        static path subPath("img");

        if (textures.find(name) == textures.end()) {
            path fullPath = resourceAt(subPath / name);
            auto texture = make_unique<Texture>(Texture::fromPath(fullPath));
            textures[name] = move(texture);
        }

        return *textures[name];
    }

    Texture * ResourceManager::getTexturePtr(const string & name) {
        return textures[name].get();
    }

    Shader & ResourceManager::getShader(const string & name) {
        static path subPath("shader");

        if (shaders.find(name) == shaders.end()) {
            path fullVertexPath = resourceAt(subPath / (name + ".vert"));
            path fullFragmentPath = resourceAt(subPath / (name + ".frag"));
            auto shader = make_unique<Shader>(
                Shader::fromPaths(fullVertexPath, fullFragmentPath));
            shaders[name] = move(shader);
        }

        return *shaders[name];
    }

    Shader * ResourceManager::getShaderPtr(const string & name) {
        return shaders[name].get();
    }

    Shader & ResourceManager::getShaderFragmentOnly(const string & name) {
        static path subpath("shader");

        if (shaders.find(name) == shaders.end()) {
            path fullFragmentPath = resourceAt(subpath / (name + ".frag"));
            auto shader =
                make_unique<Shader>(Shader::fromFragmentPath(fullFragmentPath));
            shaders[name] = move(shader);
        }

        return *shaders[name];
    }

    Model ResourceManager::loadModel(const string & name) {
        static path subPath("model");

        path fullPath = resourceAt(subPath / name);
        return Model::fromPath(fullPath);
    }

    // shared_ptr<Scene> ResourceManager::getScene(const string & name) {
    //     static path subPath("scene");

    //     if (scenes.find(name) == scenes.end()) {
    //         path fullPath = resourceAt(subPath / name);
    //         auto scene = make_shared<Scene>(Scene::fromPath(fullPath));
    //         scenes[name] = scene;
    //         return scene;
    //     }
    //     else
    //         return scenes[name];
    // }
}

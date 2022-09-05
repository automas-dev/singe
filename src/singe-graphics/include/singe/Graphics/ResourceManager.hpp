#pragma once

#include <filesystem>
#include <glpp/Shader.hpp>
#include <glpp/Texture.hpp>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Material.hpp"
#include "Scene.hpp"

namespace singe {
    using std::string;
    using std::map;
    using std::vector;
    using std::shared_ptr;
    using std::weak_ptr;
    using std::filesystem::path;
    using glpp::Shader;
    using glpp::Texture;

    class ResourceManager {
        path root;
        map<string, shared_ptr<Texture>> textures;
        map<string, shared_ptr<Shader>> shaders;
        map<string, shared_ptr<Model>> models;
        map<string, shared_ptr<Scene>> scenes;

    public:
        ResourceManager(const path & root);

        ResourceManager(ResourceManager && other);

        ResourceManager & operator=(ResourceManager && other);

        ResourceManager(const ResourceManager &) = delete;
        ResourceManager & operator=(const ResourceManager &) = delete;

        ~ResourceManager();

        void setRoot(const path & root);

        const path & getRoot() const;

        path resourceAt(const path & subPath) const;

        shared_ptr<Texture> getTexture(const string & name);

        shared_ptr<Shader> getShader(const string & name);

        shared_ptr<Shader> getShaderFragmentOnly(const string & name);

        shared_ptr<Model> getModel(const string & name);

        // shared_ptr<Scene> getScene(const string & name);
    };
}

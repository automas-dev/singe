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
    using std::unique_ptr;
    using std::weak_ptr;
    using std::filesystem::path;
    using glpp::Shader;
    using glpp::Texture;

    class ResourceManager {
        path root;
        map<string, unique_ptr<Texture>> textures;
        map<string, unique_ptr<Shader>> shaders;
        map<string, unique_ptr<Model>> models;
        map<string, unique_ptr<Scene>> scenes;

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

        Texture & getTexture(const string & name);

        [[deprecated]] Texture * getTexturePtr(const string & name);

        Shader & getShader(const string & name);

        [[deprecated]] Shader * getShaderPtr(const string & name);

        Shader & getShaderFragmentOnly(const string & name);

        Model & getModel(const string & name);

        [[deprecated]] Model * getModelPtr(const string & name);

        // shared_ptr<Scene> getScene(const string & name);
    };
}

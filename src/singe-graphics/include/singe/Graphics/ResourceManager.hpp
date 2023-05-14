#pragma once

#include <filesystem>
#include <glpp/Texture.hpp>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "Material.hpp"
#include "Mesh.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "singe/Support/log.hpp"

namespace singe::Logging {
    extern Logger::Ptr Resource;
}

namespace singe {
    using std::string;
    using std::map;
    using std::vector;
    using std::shared_ptr;
    using glpp::Texture;

    namespace fs = std::filesystem;

    class ResourceLoadException : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    /**
     * Manage path resolution, resource loading and resource caching for re-use.
     */
    class ResourceManager {
        fs::path root;
        map<string, shared_ptr<Texture>> textures;
        map<string, shared_ptr<Shader>> shaders;

    public:
        /**
         * Create a ResourceManager with all resources located at root.
         *
         * @param root the resource directory
         */
        ResourceManager(const fs::path & root);

        ResourceManager(ResourceManager && other);

        ResourceManager & operator=(ResourceManager && other);

        ResourceManager(const ResourceManager &) = delete;
        ResourceManager & operator=(const ResourceManager &) = delete;

        ~ResourceManager();

        /**
         * Update the path to the resource directory
         *
         * @param root the new resource directory
         */
        void setRoot(const fs::path & root);

        /**
         * Get the path to the current resource directory.
         *
         * @return the current resource directory
         */
        const fs::path & getRoot() const;

        /**
         * Resolve a relative path to an absolute path.
         *
         * @param subPath relative path to the resource
         *
         * @return the absolute path to the resource
         */
        fs::path resourceAt(const fs::path & subPath) const;

        /**
         * Load a glpp::Texture or return the cached texture if it exists.
         *
         * The sub path for images is /img
         *
         * @param name the texture name
         *
         * @return shared_ptr to the glpp::Texture
         */
        shared_ptr<Texture> & getTexture(const string & name);

        /**
         * Load a Shader or return the cached shader if it exists.
         *
         * The sub path for shaders is /shader
         *
         * @param name the shader name
         *
         * @return shared_ptr to the Shader
         */
        shared_ptr<Shader> & getShader(const string & name);

        /**
         * Load a Shader from fragment source only or return the cached shader
         * if it exists.
         *
         * The sub path for shaders is /shader
         *
         * @param name the shader name
         *
         * @return shared_ptr to the Shader
         */
        shared_ptr<Shader> & getShaderFragmentOnly(const string & name);

        /**
         * Load a model.
         *
         * The sub path for models is /model
         *
         * @param name the model name
         *
         * @return vector of models
         */
        vector<shared_ptr<Mesh>> loadModel(const string & name);

        // shared_ptr<Scene> getScene(const string & name);
    };
}

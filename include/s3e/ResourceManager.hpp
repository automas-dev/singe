#pragma once

#include "Shader.hpp"
#include "Model.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <unordered_map>

namespace Tom::s3e {

    /**
     * Manages resources and provides relative paths to resources based on a
     * root directory.
     *
     * Files can be accessed by using the `res://` prefix in the file path.
     */
    class ResourceManager {
    private:
        std::string rootPath;

    public:
        /**
         * Construct a ResourceManager with the current working directory as the
         * resource root path.
         */
        ResourceManager(void);

        /**
         * Construct a ResourceManager with `path` as the resource root path.
         *
         * @param path the root directory for resource paths
         */
        ResourceManager(const std::string & path);

        /**
         * Destruct the ResourceManager. If the user overrides ResourceManager,
         * all stored resources should be freed.
         */
        virtual ~ResourceManager();

        /**
         * Set the resource root path.
         *
         * @param path the new root directory for resource paths
         */
        void setResourcePath(const std::string & path);

        /**
         * Returns the current resource root directory.
         *
         * @return the current resource root directory
         */
        const std::string &getResourcePath(void);

        /**
         * Resolve the path to a resource. If `path` has the `res://` protocol
         * as it's prefix, this function with return the path relative to the
         * resource root path. If `path` is relative or absolute, then `path`
         * will be returned without modification.
         *
         * @param path the resource path
         *
         * @return the resolved resource path
         */
        std::string resourceAt(const std::string & path) const;
    };

    /**
     * An implementation of ResourceManager that has methods for loading
     * Texture, Model and Shader resources.
     */
    class DefaultResourceManager : public ResourceManager {
    private:
        std::unordered_map<std::string, Texture::Ptr> textures;

    public:
        using ResourceManager::ResourceManager;

        /**
         * Load a Texture from it's given name when first loaded.
         *
         * @param name the name of the Texture
         *
         * @return a shared_ptr to a Texture
         */
        Texture::Ptr loadTexture(const std::string & name);

        /**
         * Load a texture from `path` and cache it using `name` as it's key.
         * If you later call loadTexture with the same name, the cached Texture
         * will be returned.
         *
         * @param name the name of the Texture
         * @param path the path to the Texture files
         *
         * @return a shared_ptr to a Texture
         */
        Texture::Ptr loadTexture(const std::string & name, const std::string & path);

        /**
         * Load a Shader using `vertexPath` and `fragmentPath`.
         *
         * @param vertexPath the path to the vertex shader source
         * @param fragmentPath the path to the fragment shader source
         *
         * @return a shard_ptr to a Shader
         */
        Shader::Ptr loadShader(const std::string & vertexPath, const std::string & fragmentPath);

        /**
         * Load a Model from `path`.
         *
         * @param path the path to the obj file
         *
         * @return a shared_ptr to a Model
         */
        Model::Ptr loadModel(const std::string & path);
    };
}

#pragma once

#include <SFML/Graphics.hpp>
#include <glpp/Shader.hpp>
#include <glpp/Texture.hpp>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#include "singe/Graphics/Model.hpp"
#include "singe/Graphics/Scene.hpp"

namespace singe {
    using std::string;

    /**
     * Manages resources and provides paths to resources based on a root
     * directory.
     */
    class ResourceManagerBase {
    private:
        string rootPath;

    public:
        /**
         * Construct a ResourceManagerBase with the current working directory as
         * the resource root path.
         */
        ResourceManagerBase();

        /**
         * Construct a ResourceManagerBase with `path` as the resource root path.
         *
         * @param path the root directory for resource paths
         */
        ResourceManagerBase(const string & path);

        ResourceManagerBase(ResourceManagerBase && other);

        ResourceManagerBase & operator=(ResourceManagerBase && other);

        ResourceManagerBase(const ResourceManagerBase &) = delete;
        ResourceManagerBase & operator=(const ResourceManagerBase &) = delete;

        /**
         * Destruct the ResourceManagerBase. If the user overrides
         * ResourceManagerBase, all stored resources should be freed.
         */
        virtual ~ResourceManagerBase();

        /**
         * Set the resource root path.
         *
         * @param path the new root directory for resource paths
         */
        void setResourcePath(const string & path);

        /**
         * Returns the current resource root directory.
         *
         * @return the current resource root directory
         */
        const string & getResourcePath();

        /**
         * Resolve the path to a resource. If `path` is relative, this
         * function with return the path relative to the resource root path. If
         * `path` is absolute, then `path` will be returned without
         * modification.
         *
         * @param path the resource path
         *
         * @return the resolved resource path
         */
        string resourceAt(const string & path) const;
    };

    /**
     * An implementation of ResourceManagerBase that has methods for loading
     * Texture, Model and Shader resources.
     */
    class ResourceManager : public ResourceManagerBase {
        std::unordered_map<string, std::shared_ptr<Texture>> textures;

    public:
        using ResourceManagerBase::ResourceManagerBase;

        /**
         * Load a Texture from it's given name when first loaded.
         *
         * Note: this must be called in the same thread as the OpenGL context.
         *
         * @param path the path to the Texture files
         *
         * @return a shared_ptr to a Texture
         */
        std::shared_ptr<Texture> loadTexture(const string & path,
                                             GLint magFilter = GL_LINEAR,
                                             GLint minFilter = GL_LINEAR_MIPMAP_LINEAR);

        /**
         * Load a Shader using `vertexPath` and `fragmentPath`.
         *
         * Note: this must be called in the same thread as the OpenGL context.
         *
         * @param vertexPath the path to the vertex shader source
         * @param fragmentPath the path to the fragment shader source
         *
         * @return a shard_ptr to a Shader
         */
        std::shared_ptr<Shader> loadShader(const string & vertexPath,
                                           const string & fragmentPath);

        /**
         * Load a Shader using the default vertex shader and `fragmentPath`.
         *
         * Note: this must be called in the same thread as the OpenGL context.
         *
         * @param fragmentPath the path to the fragment shader source
         *
         * @return a shard_ptr to a Shader
         */
        std::shared_ptr<Shader> loadShader(const string & fragmentPath);

        /**
         * Load a Scene from `path`.
         *
         * Note: the user needs to call send on the resulting Scene in the
         * same thread as the OpenGL context.
         *
         * @param path the path to the obj file
         *
         * @return a shared_ptr to a Scene
         */
        Scene::Ptr loadScene(const string & path);
    };
}

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#include "s3e/Core/Scene.hpp"
#include "s3e/Core/Model.hpp"
#include "s3e/Graphics/Shader.hpp"
#include "s3e/Graphics/Texture.hpp"

namespace Tom::s3e {

    /**
     * Manages resources and provides paths to resources based on a root
     * directory.
     */
    class ResourceManagerBase {
    private:
        std::string rootPath;

    public:
        using Ptr = std::shared_ptr<ResourceManagerBase>;
        using ConstPtr = std::shared_ptr<const ResourceManagerBase>;

        /**
         * Construct a ResourceManagerBase with the current working directory as
         * the resource root path.
         */
        ResourceManagerBase(void);

        /**
         * Construct a ResourceManagerBase with `path` as the resource root path.
         *
         * @param path the root directory for resource paths
         */
        ResourceManagerBase(const std::string & path);

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
        void setResourcePath(const std::string & path);

        /**
         * Returns the current resource root directory.
         *
         * @return the current resource root directory
         */
        const std::string & getResourcePath(void);

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
        std::string resourceAt(const std::string & path) const;
    };

    struct MaterialLib {
        std::string name;
        std::map<std::string, Material::Ptr> materials;
    };

    /**
     * An implementation of ResourceManagerBase that has methods for loading
     * Texture, Model and Shader resources.
     */
    class ResourceManager : public ResourceManagerBase {
    private:
        std::unordered_map<std::string, Texture::Ptr> textures;

        std::unique_ptr<MaterialLib> loadMaterials(const std::string & objPath,
                                                   const std::string & name);

    public:
        using Ptr = std::shared_ptr<ResourceManager>;
        using ConstPtr = std::shared_ptr<const ResourceManager>;

        using ResourceManagerBase::ResourceManagerBase;

        /**
         * Load a Texture from it's given name when first loaded.
         *
         * @param path the path to the Texture files
         *
         * @return a shared_ptr to a Texture
         */
        Texture::Ptr loadTexture(const std::string & path);

        /**
         * Load a Shader using `vertexPath` and `fragmentPath`.
         *
         * @param vertexPath the path to the vertex shader source
         * @param fragmentPath the path to the fragment shader source
         *
         * @return a shard_ptr to a Shader
         */
        Shader::Ptr loadShader(const std::string & vertexPath,
                               const std::string & fragmentPath);

        /**
         * Load a Scene from `path`.
         *
         * @param path the path to the obj file
         *
         * @return a shared_ptr to a Scene
         */
        Scene::Ptr loadScene(const std::string & path);
    };
}

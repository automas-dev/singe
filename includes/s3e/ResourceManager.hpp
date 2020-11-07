#pragma once

#include "Shader.hpp"
#include "Model.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <unordered_map>

namespace Tom::s3e {

    class ResourceManager {
    private:
        std::unordered_map<std::string, Texture::Ptr> textures;
        std::string resRoot;

    public:
        ResourceManager(void);
        ResourceManager(const std::string & path);
        virtual ~ResourceManager();

        void setResourcePath(const std::string & path);

        Texture::Ptr loadTexture(const std::string & name);
        Texture::Ptr loadTexture(const std::string & name, const std::string & path);

        Shader::Ptr loadShader(const std::string & vertexPath, const std::string & fragmentPath);
        Model::Ptr loadModel(const std::string & path);

    protected:
        std::string resPath(const std::string & path) const;
    };
}
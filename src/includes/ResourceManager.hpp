#pragma once

#include "Shader.hpp"
#include "Model.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <map>

namespace game {

    class ResourceManager {
    private:
        std::map<sf::String, std::shared_ptr<Texture>> textures;
        sf::String resPath;

    public:
        ResourceManager(void);
        ResourceManager(const sf::String & resPath);
        virtual ~ResourceManager();

        void setResourcePath(const sf::String & path);

        Texture::Ptr loadTexture(const sf::String & name);
        Texture::Ptr loadTexture(const sf::String & name, const sf::String & path);

        Shader::Ptr loadShader(const std::string & vertexPath, const std::string & fragmentPath);
        Model::Ptr loadModel(const std::string & path);

        sf::String parsePath(const sf::String & path) const;
    };
}
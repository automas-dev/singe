#pragma once

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glpp/Shader.hpp>
#include <glpp/Texture.hpp>
#include <memory>
#include <string>

namespace singe {
    using std::string;
    using std::shared_ptr;
    using glpp::Shader;
    using glpp::Texture;
    using glm::vec3;
    using sf::Image;

    /**
     * Material for a Model.
     */
    struct Material {
        string name;
        vec3 ambient, diffuse, specular;
        float specExp;
        float alpha;
        Image image;
        shared_ptr<Texture> texture;
        shared_ptr<Texture> normalTexture;
        shared_ptr<Texture> specularTexture;

        /**
         * Create a new Material with default values.
         */
        Material();

        /**
         * Create the texture and send the image to it.
         */
        void send();

        /**
         * If texture is not a nullptr, call it's bind method.
         */
        void bind(const Shader & shader) const;
    };
}

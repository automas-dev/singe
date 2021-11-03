#pragma once

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "singe/Graphics/Shader.hpp"
#include "singe/Graphics/Texture.hpp"

namespace singe {
    /**
     * Material for a Model.
     */
    struct Material {
        using Ptr = std::shared_ptr<Material>;
        using ConstPtr = std::shared_ptr<const Material>;

        std::string name;
        glm::vec3 ambient, diffuse, specular;
        float specExp;
        float alpha;
        sf::Image image;
        Texture::Ptr texture;

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
        void bind(const Shader::Ptr & shader) const;
    };
}

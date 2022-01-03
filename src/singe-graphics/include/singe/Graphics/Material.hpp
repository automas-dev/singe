#pragma once

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string>

#include <glpp/Shader.hpp>
#include <glpp/Texture.hpp>

namespace singe {
    using glpp::Shader;
    using glpp::Texture;
    using std::shared_ptr;

    /**
     * Material for a Model.
     */
    struct Material {
        using Ptr = shared_ptr<Material>;
        using ConstPtr = shared_ptr<const Material>;

        std::string name;
        glm::vec3 ambient, diffuse, specular;
        float specExp;
        float alpha;
        sf::Image image;
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

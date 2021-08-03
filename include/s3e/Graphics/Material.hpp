#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "Texture.hpp"

namespace Tom::s3e {
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
        Texture::Ptr texture;

        /**
         * Create a new Material with default values.
         */
        Material();

        /**
         * If texture is not a nullptr, call it's bind method.
         */
        void bind() const;
    };
}

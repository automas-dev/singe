#pragma once

#include <glm/glm.hpp>
#include <glpp/Texture.hpp>
#include <memory>
#include <string>

#include "Shader.hpp"

namespace singe {
    using std::shared_ptr;
    using std::string;
    using glm::vec3;
    using glpp::Texture;

    /**
     * Material properties, textures and shader.
     */
    struct Material {
        using Ptr = shared_ptr<Material>;
        using ConstPtr = const shared_ptr<Material>;

        Shader::Ptr shader;

        string       name;
        vec3         ambient, diffuse, specular;
        float        specExp;
        float        alpha;
        Texture::Ptr texture;
        Texture::Ptr normalTexture;
        Texture::Ptr specularTexture;

        Material();

        Material(Material && other);

        Material & operator=(Material && other);

        Material(const Material &) = delete;
        Material & operator=(const Material &) = delete;

        ~Material();

        /**
         * Bind the shader and textures.
         */
        void bind() const;
    };
}

#include "s3e/Graphics/Material.hpp"

namespace Tom::s3e {
    Material::Material()
        : name("None"),
          ambient(0.8),
          diffuse(0.8),
          specular(0.8),
          specExp(500),
          alpha(1.0),
          texture(nullptr) {}

    void Material::bind() const {
        if (texture)
            texture->bind();
    }
};

#include "s3e/Core/Material.hpp"

namespace Tom::s3e {
    Material::Material()
        : name("None"),
          ambient(0.8),
          diffuse(0.8),
          specular(0.8),
          specExp(500),
          alpha(1.0),
          texture(nullptr) {}

    void Material::send() {
        if (!texture) {
            texture = std::make_shared<Texture>(image);
        }
        else {
            texture->loadFrom(image);
        }
    }

    void Material::bind() const {
        if (texture)
            texture->bind();
    }
}

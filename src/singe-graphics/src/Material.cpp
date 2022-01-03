#include "singe/Graphics/Material.hpp"

namespace singe {
    using std::make_shared;

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
            texture = make_shared<Texture>(image);
        }
        else {
            texture->loadFrom(image.getPixelsPtr(), {image.getSize().x, image.getSize().y}, 3);
        }
    }

    void Material::bind(const Shader & shader) const {
        if (texture)
            texture->bind();
    }
}

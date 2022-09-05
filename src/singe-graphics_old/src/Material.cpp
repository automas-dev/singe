#include "singe/Graphics/Material.hpp"

namespace singe {
    using std::make_shared;
    using glm::uvec2;

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
            texture = make_shared<Texture>(
                image.getPixelsPtr(),
                uvec2(image.getSize().x, image.getSize().y), 3);
        }
        else {
            texture->loadFrom(image.getPixelsPtr(),
                              {image.getSize().x, image.getSize().y}, 3);
        }
    }

    void Material::bind(const Shader & shader) const {
        if (texture)
            texture->bind();
    }
}

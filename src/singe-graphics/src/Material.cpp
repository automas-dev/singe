#include "singe/Graphics/Material.hpp"

#include <memory>

namespace singe {
    using std::move;

    Material::Material() {}

    Material::Material(Material && other)
        : name(std::move(other.name)),
          ambient(std::move(other.ambient)),
          diffuse(std::move(other.diffuse)),
          specular(std::move(other.specular)),
          specExp(other.specExp),
          alpha(other.alpha),
          texture(std::move(other.texture)),
          normalTexture(std::move(other.normalTexture)),
          specularTexture(std::move(other.specularTexture)) {}

    Material & Material::operator=(Material && other) {
        name = std::move(other.name);
        ambient = std::move(other.ambient);
        diffuse = std::move(other.diffuse);
        specular = std::move(other.specular);
        specExp = other.specExp;
        alpha = other.alpha;
        texture = std::move(other.texture);
        normalTexture = std::move(other.normalTexture);
        specularTexture = std::move(other.specularTexture);
        return *this;
    }

    Material::~Material() {}

    void Material::bind() const {
        if (texture) {
            glActiveTexture(GL_TEXTURE0);
            texture->bind();
        }

        if (normalTexture) {
            glActiveTexture(GL_TEXTURE1);
            normalTexture->bind();
        }

        if (specularTexture) {
            glActiveTexture(GL_TEXTURE2);
            specularTexture->bind();
        }
    }
}

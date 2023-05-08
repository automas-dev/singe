#include "singe/Graphics/Material.hpp"

#include <memory>

namespace singe {
    using std::move;

    Material::Material() {}

    Material::Material(Material && other)
        : shader(other.shader),
          name(move(other.name)),
          ambient(move(other.ambient)),
          diffuse(move(other.diffuse)),
          specular(move(other.specular)),
          specExp(other.specExp),
          alpha(other.alpha),
          texture(move(other.texture)),
          normalTexture(move(other.normalTexture)),
          specularTexture(move(other.specularTexture)) {}

    Material & Material::operator=(Material && other) {
        shader = other.shader;
        name = move(other.name);
        ambient = move(other.ambient);
        diffuse = move(other.diffuse);
        specular = move(other.specular);
        specExp = other.specExp;
        alpha = other.alpha;
        texture = move(other.texture);
        normalTexture = move(other.normalTexture);
        specularTexture = move(other.specularTexture);
        return *this;
    }

    Material::~Material() {}

    void Material::bind() const {
        if (shader)
            shader->bind();

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

#include "singe/Graphics/Material.hpp"

#include <memory>

namespace singe {
    using std::move;

    Material::Material() {}

    Material::Material(Material && other) {}

    Material & Material::operator=(Material && other) {
        return *this;
    }

    Material::~Material() {}
}

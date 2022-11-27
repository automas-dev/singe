#include "singe/Graphics/Geometry.hpp"

#include <memory>

namespace singe {
    using std::move;

    Geometry::Geometry(const Material * material) : material(material) {}

    Geometry::Geometry(Geometry && other)
        : mesh(move(other.mesh)), material(other.material) {}

    Geometry & Geometry::operator=(Geometry && other) {
        mesh = move(other.mesh);
        material = other.material;
        return *this;
    }

    Geometry::~Geometry() {}

    Mesh & Geometry::getMesh() {
        return mesh;
    }

    void Geometry::draw(RenderState & state) const {
        if (material && material->texture)
            material->texture->bind();
        mesh.draw(state);
    }
}

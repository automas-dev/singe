#include "singe/Graphics/Geometry.hpp"

#include <memory>

namespace singe {
    using std::move;

    Geometry::Geometry(size_t materialId) : materialId(materialId) {}

    Geometry::Geometry(Mesh && mesh, size_t materialId)
        : mesh(move(mesh)), materialId(materialId) {}

    Geometry::Geometry(Geometry && other)
        : mesh(move(other.mesh)), materialId(materialId) {}

    Geometry & Geometry::operator=(Geometry && other) {
        mesh = move(other.mesh);
        materialId = other.materialId;
        return *this;
    }
}
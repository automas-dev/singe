#include "singe/Graphics/Geometry.hpp"

namespace singe {
    Geometry::Geometry(const Mesh::Ptr & mesh, size_t materialId)
        : mesh(mesh), materialId(materialId) {}
}
#include "singe/Core/Geometry.hpp"

namespace singe {
    Geometry::Geometry(const Mesh::Ptr & mesh, size_t materialIndex)
        : mesh(mesh), materialIndex(materialIndex) {}
}
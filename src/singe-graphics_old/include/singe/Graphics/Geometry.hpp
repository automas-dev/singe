#pragma once

#include <memory>

#include "singe/Graphics/Mesh.hpp"

namespace singe {
    struct Geometry {
        Mesh mesh;
        size_t materialId;

        Geometry(size_t materialId = 0);
        
        Geometry(Mesh && mesh, size_t materialId = 0);

        Geometry(Geometry && other);

        Geometry & operator=(Geometry && other);

        Geometry(const Geometry &) = delete;
        Geometry & operator=(const Geometry &) = delete;
    };
}

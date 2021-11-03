#pragma once

#include <memory>

#include "Mesh.hpp"

namespace singe {
    struct Geometry {
        Mesh::Ptr mesh;
        size_t materialIndex;

        using Ptr = std::shared_ptr<Geometry>;
        using ConstPtr = std::shared_ptr<const Geometry>;

        Geometry(const Mesh::Ptr & mesh, size_t materialIndex = 0);
        
        Geometry(const Geometry & other) = default;
        
        Geometry(Geometry && other) = default;
        
        Geometry & operator=(const Geometry & other) = default;
        
        Geometry & operator=(Geometry && other) = default;
    };
}

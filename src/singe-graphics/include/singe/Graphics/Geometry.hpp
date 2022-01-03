#pragma once

#include <memory>

#include "singe/Graphics/Mesh.hpp"

namespace singe {
    struct Geometry {
        Mesh::Ptr mesh;
        size_t materialId;

        using Ptr = std::shared_ptr<Geometry>;
        using ConstPtr = std::shared_ptr<const Geometry>;

        Geometry(const Mesh::Ptr & mesh, size_t materialId = 0);

        Geometry(const Geometry & other) = default;

        Geometry(Geometry && other) = default;

        Geometry & operator=(const Geometry & other) = default;

        Geometry & operator=(Geometry && other) = default;
    };
}

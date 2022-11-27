#pragma once

#include <glm/glm.hpp>

#include "Material.hpp"
#include "Mesh.hpp"
#include "RenderState.hpp"

namespace singe {
    struct Geometry {
        Mesh mesh;
        const Material * material;

        Geometry(const Material * material);

        Geometry(Geometry && other);

        Geometry & operator=(Geometry && other);

        Geometry(const Geometry &) = delete;
        Geometry & operator=(const Geometry &) = delete;

        ~Geometry();

        Mesh & getMesh();

        void draw(RenderState & state) const;
    };
}

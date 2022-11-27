#pragma once

#include <string>
#include <vector>

#include "Geometry.hpp"
#include "Material.hpp"
#include "RenderState.hpp"

namespace singe {
    using std::string;
    using std::vector;

    struct Model {
        vector<Material> materials;
        vector<Geometry> geometry;
        Transform transform;

        Model();

        Model(Model && other);

        Model & operator=(Model && other);

        Model(const Model &) = delete;
        Model & operator=(const Model &) = delete;

        ~Model();

        void draw(RenderState state) const;

        static Model fromPath(const string & path);
    };
}

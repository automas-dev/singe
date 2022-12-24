#pragma once

#include <glpp/extra/Transform.hpp>
#include <memory>
#include <vector>

#include "Model.hpp"
#include "RenderState.hpp"

namespace singe {
    using std::shared_ptr;
    using std::vector;
    using glpp::extra::Transform;

    struct Scene {
        vector<Scene> children;
        vector<Model> models;
        Transform transform;

        Scene();

        Scene(Scene && other);

        Scene & operator=(Scene && other);

        Scene(const Scene &) = delete;
        Scene & operator=(const Scene &) = delete;

        ~Scene();

        void draw(RenderState state) const;
    };
}

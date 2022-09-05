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

    class Scene {
        vector<shared_ptr<Scene>> children;
        vector<shared_ptr<Model>> models;
        Transform transform;

    public:
        Scene();

        Scene(Scene && other);

        Scene & operator=(Scene && other);

        Scene(const Scene &) = delete;
        Scene & operator=(const Scene &) = delete;

        ~Scene();

        void draw(RenderState state) const;
    };
}

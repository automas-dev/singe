#pragma once

#include <glpp/extra/Transform.hpp>
#include <memory>
#include <vector>

#include "Mesh.hpp"
#include "RenderState.hpp"

namespace singe {
    using std::shared_ptr;
    using std::vector;
    using glpp::extra::Transform;

    /**
     * Group of Mesh and child Scenes.
     */
    struct Scene {
        vector<shared_ptr<Scene>> children;
        vector<shared_ptr<Mesh>> models;
        Transform transform;

        Scene();

        Scene(Scene && other);

        Scene & operator=(Scene && other);

        Scene(const Scene &) = delete;
        Scene & operator=(const Scene &) = delete;

        ~Scene();

        /**
         * Draw child scenes and then mesh in this scene.
         *
         * Mesh in this scene will be drawn with this transform and child
         * scenes will transform with this scene as their origin.
         *
         * @param state the RenderState with the current global transform
         */
        void draw(RenderState state) const;
    };
}

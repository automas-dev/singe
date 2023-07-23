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

    /**
     * Group of Models and child Scenes.
     */
    struct Scene {
        vector<shared_ptr<Scene>> children;
        vector<shared_ptr<Model>> models;
        Transform transform;

        Scene();

        Scene(Scene && other);

        Scene & operator=(Scene && other);

        Scene(const Scene &) = delete;
        Scene & operator=(const Scene &) = delete;

        ~Scene();

        /**
         * Create and return a new child Scene.
         *
         * @return reference to the new Scene
         */
        shared_ptr<Scene> & addChild();

        /**
         * Create and return a new model.
         *
         * @return reference to the new model
         */
        shared_ptr<Model> & addModel();

        /**
         * Draw child scenes and then mesh in this scene.
         *
         * Models in this scene will be drawn with this transform and child
         * scenes will transform with this scene as their origin.
         *
         * @param state the RenderState with the current global transform
         */
        void draw(RenderState state) const;
    };
}

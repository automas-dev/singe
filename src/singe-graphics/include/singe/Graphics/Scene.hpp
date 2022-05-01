#pragma once

#include <glpp/extra/Transform.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

#include "glpp/Shader.hpp"
#include "singe/Graphics/Model.hpp"
#include "singe/Graphics/RenderState.hpp"

namespace singe {
    using glpp::extra::Transform;

    /**
     * A collection of models that share a transform.
     *
     * The transforms also apply to all children.
     */
    struct Scene : public Transform {

        using Ptr = std::shared_ptr<Scene>;
        using ConstPtr = std::shared_ptr<const Scene>;

        std::string name;
        std::vector<Model::Ptr> models;
        std::vector<Scene::Ptr> children;

        /**
         * Create a new scene with name,
         *
         * @param the scene name
         */
        Scene(const std::string & name);

        /**
         * Draw the scene.
         *
         * Sends the model matrix to shader at the uniform named "model".
         *
         * @param shader the shader to send the model matrix to
         * @param transform any parent transform
         */
        void draw(RenderState state) const;
    };
}

#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

#include "s3e/Core/Model.hpp"
#include "s3e/Graphics/Shader.hpp"
#include "s3e/Support/Transform3d.hpp"

namespace Tom::s3e {

    /**
     * A collection of models that share a transform.
     *
     * The transforms also apply to all children.
     */
    struct Scene : public Transform3d {

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
        void draw(const Shader::Ptr & shader,
                  glm::mat4 transform = glm::mat4(1)) const;
    };
}

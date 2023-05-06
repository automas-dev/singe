#pragma once

#include <glm/glm.hpp>
#include <glpp/Shader.hpp>
#include <glpp/extra/Transform.hpp>

#include "Shader.hpp"

namespace singe {
    using glm::mat4;
    using glpp::extra::Transform;

    /**
     * Store transform, shader and mvp uniform. Pass this by value to create
     * a stack effect of applying transforms in layers.
     */
    struct RenderState {
        mat4 transform;

        /**
         * Create a RenderState with initial transform.
         */
        RenderState(const mat4 & transform);

        ~RenderState();

        /**
         *  Multiply this matrix with the matrix of another Transform.
         */
        void pushTransform(const Transform & transform);

        /**
         * Multiply this matrix with another matrix.
         *
         * @param matrix the matrix used to multiply transform
         */
        void pushTransform(const mat4 & matrix);

        /**
         * Apply the current transform to the Shader mvp uniform.
         *
         * @param shader the Shader with mvp uniform
         */
        // void apply(const Shader * shader);

        /**
         * Apply the current transform to the glpp::Uniform mvp.
         *
         * @param mvp the glpp::Uniform for mvp which is mat4
         */
        void apply(const glpp::Uniform & mvp);
    };
}

#pragma once

#include <glm/glm.hpp>
#include <glpp/Shader.hpp>

namespace singe {
    using glpp::Shader;
    using glpp::Uniform;

    struct RenderState {
        const Shader & shader;
        Uniform mvp;
        glm::mat4 transform;

        RenderState(const glpp::Shader & shader, const glm::mat4 & transform);

        /**
         * Default copy constructor.
         */
        RenderState(const RenderState & other) = default;

        /**
         * Default move constructor.
         */
        RenderState(RenderState && other) = default;

        /**
         * Default copy assign operator.
         */
        RenderState & operator=(const RenderState & other) = default;

        /**
         * Default move assign operator.
         */
        RenderState & operator=(RenderState && other) = default;

        /**
         * Multiply the current transform by `transform` and send it to the
         * shader.
         */
        void pushTransform(const glm::mat4 & transform);
    };
}
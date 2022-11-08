#pragma once

#include <glm/glm.hpp>
#include <glpp/Shader.hpp>
#include <glpp/extra/Transform.hpp>

namespace singe {
    using glm::mat4;
    using glpp::Uniform;
    using glpp::Shader;
    using glpp::extra::Transform;

    struct RenderState {
        mat4 transform;
        const Shader & shader;
        const Uniform mvp;

        RenderState(const mat4 & transform, const Shader & shader);

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

        ~RenderState();

        void pushTransform(const Transform & transform);

        void pushTransform(const mat4 & matrix);
    };
}

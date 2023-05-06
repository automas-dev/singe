#pragma once

#include <glm/glm.hpp>
#include <glpp/Shader.hpp>
#include <glpp/extra/Transform.hpp>

namespace singe {
    using glm::mat4;
    using glpp::Uniform;
    using glpp::Shader;
    using glpp::extra::Transform;

    /**
     * Store transform, shader and mvp uniform. Pass this by value to create
     * a stack effect of applying transforms in layers.
     */
    struct RenderState {
        mat4 transform;
        const Shader & shader;
        const Uniform mvp;

        /**
         * Create a RenderState with initial transform and shader. The shader
         * will be used to get the mvp uniform.
         */
        RenderState(const mat4 & transform, const Shader & shader);

        ~RenderState();

        void pushTransform(const Transform & transform);

        void pushTransform(const mat4 & matrix);
    };
}

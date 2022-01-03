#pragma once

#include <Transform3d.hpp>
#include <glm/glm.hpp>
#include <glpp/Shader.hpp>

#include "singe/Core/Camera.hpp"

namespace singe {
    struct RenderState {
        const Camera::Ptr & camera;
        const glpp::Shader & shader;
        glm::mat4 transform;

        RenderState(const Camera::Ptr & camera,
                    const glpp::Shader & shader,
                    glm::mat4 & transform);

        RenderState(const RenderState & other) = default;

        RenderState(RenderState && other) = default;

        RenderState & operator=(const RenderState & other) = default;

        RenderState & operator=(RenderState && other) = default;

        void sendMVP() const;
    };
}
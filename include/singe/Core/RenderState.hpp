#pragma once

#include <Transform3d.hpp>
#include <glm/glm.hpp>

#include "singe/Core/Camera.hpp"
#include "singe/Graphics/Shader.hpp"

namespace singe {
    struct RenderState {
        Camera & camera;
        Shader & shader;
        glm::mat4 transform;

        RenderState(Camera & camera, Shader & shader, glm::mat4 & transform);

        RenderState(const RenderState & other) = default;

        RenderState(RenderState && other) = default;

        RenderState & operator=(const RenderState & other) = default;

        RenderState & operator=(RenderState && other) = default;
    };
}
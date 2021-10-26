#pragma once

#include "singe/Core/Camera.hpp"
#include "singe/Graphics/Shader.hpp"
#include "singe/Support/Transform3d.hpp"

namespace singe {
    struct RenderState {
        Camera::Ptr & camera;
        Shader::Ptr & shader;
        Transform3d transform;

        RenderState(Camera::Ptr & camera,
                    Shader::Ptr & shader,
                    Transform3d & transform);

        RenderState(const RenderState & other) = default;

        RenderState(RenderState && other) = default;

        RenderState & operator=(const RenderState & other) = default;

        RenderState & operator=(RenderState && other) = default;
    };
}
#include "singe/Core/RenderState.hpp"

namespace singe {
    RenderState::RenderState(Camera::Ptr & camera,
                             Shader::Ptr & shader,
                             Transform3d & transform)
        : camera(camera), shader(shader), transform(transform) {}
}
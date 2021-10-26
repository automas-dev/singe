#include "singe/Core/RenderState.hpp"

namespace singe {
    RenderState::RenderState(Camera & camera, Shader & shader, glm::mat4 & transform)
        : camera(camera), shader(shader), transform(transform) {}
}
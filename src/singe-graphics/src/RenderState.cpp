#include "singe/Graphics/RenderState.hpp"

namespace singe {
    RenderState::RenderState(const glpp::Shader & shader, glm::mat4 & transform)
        : shader(shader), transform(transform) {}

    void RenderState::sendMVP() const {
        shader.setMat4("mvp", transform);
    }
}
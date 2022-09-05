#include "singe/Graphics/RenderState.hpp"

#include <memory>

namespace singe {
    RenderState::RenderState(const glpp::Shader & shader, const glm::mat4 & transform)
        : shader(shader), mvp(shader.uniform("mvp")), transform(transform) {}

    void RenderState::pushTransform(const glm::mat4 & transform) {
        this->transform *= transform;
        shader.bind();
        mvp.setMat4(this->transform);
    }
}
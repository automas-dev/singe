#include "singe/Core/RenderState.hpp"

namespace singe {
    RenderState::RenderState(const Camera::Ptr & camera,
                             const Shader::Ptr & shader,
                             glm::mat4 & transform)
        : camera(camera), shader(shader), transform(transform) {}

    void RenderState::sendMVP() const {
        shader->setMat4("mvp", transform);
    }

    void RenderState::sendModel(const glm::mat4 & matrix) const {
        shader->setMat4("model", matrix);
    }
}
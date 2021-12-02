#include "singe/Graphics/RenderState.hpp"

namespace singe {
    RenderState::RenderState(const Camera::Ptr & camera,
                             const glpp::Shader & shader,
                             glm::mat4 & transform)
        : camera(camera), shader(shader), transform(transform) {}

    void RenderState::sendMVP() const {
        shader.setMat4("mvp", transform);
    }
}
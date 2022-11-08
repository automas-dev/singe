#include "singe/Graphics/RenderState.hpp"

namespace singe {
    RenderState::RenderState(const mat4 & transform, const Shader & shader)
        : transform(transform), shader(shader), mvp(shader.uniform("mvp")) {}

    RenderState::~RenderState() {}

    void RenderState::pushTransform(const Transform & transform) {
        pushTransform(transform.toMatrix());
    }

    void RenderState::pushTransform(const mat4 & matrix) {
        transform *= matrix;
        // shader.bind();
        mvp.setMat4(transform);
    }
}

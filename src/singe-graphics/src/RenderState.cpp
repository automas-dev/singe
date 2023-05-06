#include "singe/Graphics/RenderState.hpp"

namespace singe {
    RenderState::RenderState(const mat4 & transform) : transform(transform) {}

    RenderState::~RenderState() {}

    void RenderState::pushTransform(const Transform & transform) {
        pushTransform(transform.toMatrix());
    }

    void RenderState::pushTransform(const mat4 & matrix) {
        transform *= matrix;
    }

    // void RenderState::apply(const Shader * shader) {
    //     if (shader)
    //         apply(shader->mvp());
    // }

    void RenderState::apply(const glpp::Uniform & uniform) {
        uniform.setMat4(transform);
    }
}

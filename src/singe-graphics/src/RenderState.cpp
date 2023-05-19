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
}

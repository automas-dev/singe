#include "singe/Graphics/RenderState.hpp"

namespace singe {
    RenderState::RenderState() : projection(1), view(1), model(1), local(1) {}

    RenderState::RenderState(const mat4 & projection,
                             const mat4 & view,
                             const mat4 & model,
                             const mat4 & local)
        : projection(projection), view(view), model(model), local(local) {}

    RenderState::RenderState(const Camera & camera,
                             const mat4 & model,
                             const mat4 & local)
        : projection(camera.projMatrix()),
          view(camera.viewMatrix()),
          model(model),
          local(local) {}

    RenderState::~RenderState() {}

    mat4 RenderState::getVP() const {
        return projection * view;
    }

    mat4 RenderState::getMVP() const {
        return getVP() * model;
    }

    const mat4 & RenderState::getModel() const {
        return model;
    }

    const mat4 & RenderState::getLocal() const {
        return local;
    }

    void RenderState::pushTransform(const Transform & transform) {
        pushTransform(transform.toMatrix());
    }

    void RenderState::pushTransform(const mat4 & matrix) {
        model *= matrix;
        local = matrix;
    }
}

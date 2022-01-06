#include "singe/Graphics/Scene.hpp"

#include "singe/Support/log.hpp"

namespace singe {
    Scene::Scene(const std::string & name) : name(name) {}

    void Scene::draw(RenderState state) const {
        state.transform *= toMatrix();
        for (auto & model : models) {
            model->draw(state);
        }
        for (auto & child : children) {
            child->draw(state);
        }
    }
}

#include "singe/Core/Scene.hpp"

#include "singe/Support/log.hpp"

namespace singe {
    Scene::Scene(const std::string & name) : name(name) {}

    void Scene::send() {
        for (auto & model : models) {
            model->send();
        }
        for (auto & scene : children) {
            scene->send();
        }
    }

    void Scene::draw(RenderState state) const {
        state.transform *= toMatrix();
        state.sendMVP();
        for (auto & model : models) {
            model->draw(state);
        }
        for (auto & child : children) {
            child->draw(state);
        }
    }
}

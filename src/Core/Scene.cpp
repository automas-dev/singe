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

    void Scene::draw(const Shader::Ptr & shader, glm::mat4 transform) const {
        transform *= toMatrix();
        shader->setMat4("model", transform);
        for (auto & model : models) {
            model->draw();
        }
        for (auto & child : children) {
            child->draw(shader, transform);
        }
    }
}

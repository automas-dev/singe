#include "s3e/Core/Scene.hpp"

#include "s3e/Support/log.hpp"

namespace Tom::s3e {
    Scene::Scene(const std::string & name) : name(name) {}

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

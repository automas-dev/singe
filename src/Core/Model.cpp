#include "singe/Core/Model.hpp"

#include "singe/Graphics/VBO.hpp"
#include "singe/Support/Util.hpp"
#include "singe/Support/log.hpp"

namespace singe {

    Model::Model(const std::string & name) : Mesh(), name(name) {}

    Model::~Model() {}

    void Model::send() {
        Mesh::send();
        for (auto & material : materials) {
            material->send();
        }
    }

    void Model::draw() const {
        for (int i = 0; i < materials.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            materials[i]->bind();
        }
        Mesh::draw();
    }
}

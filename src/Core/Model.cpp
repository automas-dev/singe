#include "singe/Core/Model.hpp"

#include "singe/Graphics/VBO.hpp"
#include "singe/Support/Util.hpp"
#include "singe/Support/log.hpp"

namespace singe {

    Model::Model(const std::string & name) : Transform3d(), name(name) {}

    Model::~Model() {}

    void Model::send() {
        for (auto & geom : geometry) {
            geom.mesh->send();
        }
    }

    void Model::draw(RenderState state) const {
        for (auto & geom : geometry) {
            state.sendModel(toMatrix());
            materials[geom.materialId]->bind(state.shader);
            geom.mesh->draw();
        }
    }
}

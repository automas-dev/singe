#include "singe/Graphics/Model.hpp"

#include <singe/Support/Util.hpp>
#include <singe/Support/log.hpp>

namespace singe {
    using std::move;

    Model::Model(const string & name) : name(name) {}

    Model::Model(Model && other)
        : geometry(move(other.geometry)),
          materials(move(other.materials)),
          name(move(other.name)),
          transform(move(other.transform)) {}

    Model & Model::operator=(Model && other) {
        geometry = move(other.geometry);
        materials = move(other.materials);
        name = move(other.name);
        transform = move(other.transform);
        return *this;
    }

    Model::~Model() {}

    void Model::draw(Buffer::Mode mode, RenderState state) const {
        state.pushTransform(transform.toMatrix());
        for (auto & geom : geometry) {
            materials[geom.materialId]->bind(state.shader);
            geom.mesh.draw(mode);
        }
    }
}

#include "singe/Graphics/Model.hpp"

#include <memory>

namespace singe {
    using std::move;

    Model::Model() : material(nullptr) {}

    Model::Model(const vector<Vertex> & points)
        : points(points), material(nullptr) {
        update();
    }

    Model::Model(vector<Vertex> && points)
        : points(move(points)), material(nullptr) {
        update();
    }

    Model::Model(Model && other)
        : points(move(other.points)),
          array(move(other.array)),
          material(other.material),
          transform(other.transform) {}

    Model & Model::operator=(Model && other) {
        points = move(other.points);
        array = move(other.array);
        material = other.material;
        transform = other.transform;
        return *this;
    }

    Model::~Model() {}

    void Model::update(Buffer::Usage usage) {
        array.bufferData(points, usage);
        array.unbind();
    }

    void Model::draw(RenderState state) const {
        state.pushTransform(transform);
        if (material) {
            material->bind();
            if (material->shader)
                material->shader->bind(state);
        }
        array.drawArrays(Buffer::Triangles, 0, points.size());
    }
}

#include "singe/Graphics/Mesh.hpp"

#include <memory>

namespace singe {
    using std::move;

    Mesh::Mesh() : material(nullptr) {}

    Mesh::Mesh(const vector<Vertex> & points)
        : points(points), material(nullptr) {
        update();
    }

    Mesh::Mesh(vector<Vertex> && points)
        : points(move(points)), material(nullptr) {
        update();
    }

    Mesh::Mesh(Mesh && other)
        : points(move(other.points)),
          array(move(other.array)),
          material(other.material),
          transform(other.transform) {}

    Mesh & Mesh::operator=(Mesh && other) {
        points = move(other.points);
        array = move(other.array);
        material = other.material;
        transform = other.transform;
        return *this;
    }

    Mesh::~Mesh() {}

    void Mesh::update(Buffer::Usage usage) {
        array.bufferData(points, usage);
        array.unbind();
    }

    void Mesh::draw(RenderState state) const {
        state.pushTransform(transform);
        if (material) {
            material->bind();
            if (material->shader)
                state.apply(material->shader->mvp());
                // material->shader->mvp().setMat4(transform.toMatrix());
            // state.apply(material->shader.get());
        }
        array.drawArrays(Buffer::Triangles, 0, points.size());
    }
}

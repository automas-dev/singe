#include "singe/Graphics/Mesh.hpp"

#include <memory>

namespace singe {
    using std::move;

    Mesh::Mesh() : points(), array() {}

    Mesh::Mesh(const vector<Vertex> & points) : points(points), array() {
        update();
    }

    Mesh::Mesh(vector<Vertex> && points) : points(move(points)), array() {
        update();
    }

    Mesh::Mesh(Mesh && other)
        : points(move(other.points)), array(move(other.array)) {}

    Mesh & Mesh::operator=(Mesh && other) {
        points = move(other.points);
        array = move(other.array);
        return *this;
    }

    Mesh::~Mesh() {}

    void Mesh::update(Buffer::Usage usage) {
        array.bufferData(points, usage);
        array.unbind();
    }

    void Mesh::draw(RenderState & state) const {
        array.drawArrays(Buffer::Triangles, 0, points.size());
    }
}

#include "singe/Graphics/Mesh.hpp"

#include <GL/glew.h>
// gl.h after glew.h, clang-format don't sort
#include <GL/gl.h>

#include <memory>
#include <vector>

namespace singe {
    using std::move;
    using std::vector;
    using glpp::Attribute;
    using glpp::extra::Vertex;

    Mesh::Mesh() : array(), n(0) {}

    Mesh::Mesh(Mesh && other) : array(move(other.array)), n(other.n) {}

    Mesh & Mesh::operator=(Mesh && other) {
        array = move(other.array);
        n = other.n;
        return *this;
    }

    Mesh::~Mesh() {}

    void Mesh::bufferData(const vector<Vertex> & data, Buffer::Usage usage) {
        array.bufferData(data, usage);
        n = data.size();
        array.unbind();
    }

    void Mesh::draw(RenderState & state) const {
        array.drawArrays(Buffer::Triangles, n, 0);
    }
}

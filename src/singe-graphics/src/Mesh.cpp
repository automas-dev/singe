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

    static const vector<Attribute> attrs {
        {0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0},
        {1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(3 * sizeof(float))},
        {2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(6 * sizeof(float))},
    };

    Mesh::Mesh() : array({attrs}), n(0) {}

    Mesh::Mesh(Mesh && other) : array(move(other.array)), n(other.n) {}

    Mesh & Mesh::operator=(Mesh && other) {
        array = move(other.array);
        n = other.n;
        return *this;
    }

    Mesh::~Mesh() {}

    void Mesh::bufferData(const vector<Vertex> & data, Buffer::Usage usage) {
        array.bufferData(0, data.size() * sizeof(Vertex), data.data(), usage);
        n = data.size();
    }

    void Mesh::draw(RenderState & state) const {
        array.drawArrays(Buffer::Triangles, n, 0);
    }
}

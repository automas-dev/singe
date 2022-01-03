#include "singe/Graphics/Mesh.hpp"

#include "singe/Support/log.hpp"
using namespace glpp;

namespace singe {
    Mesh::Mesh() {}

    Mesh::~Mesh() {}

    void Mesh::loadFromPoints(const std::vector<Vertex> & points) {
        buffer.loadFromPoints(points);
    }

    void Mesh::appendPoints(const std::vector<Vertex> & points) {
        buffer.buff.insert(buffer.buff.end(), points.begin(), points.end());
    }

    void Mesh::draw(glpp::Buffer::Mode mode) const {
        buffer.draw(mode);
    }
}

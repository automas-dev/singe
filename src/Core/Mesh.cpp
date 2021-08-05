#include "s3e/Core/Mesh.hpp"

namespace Tom::s3e {
    Mesh::~Mesh() {}

    void Mesh::loadFromPoints(const std::vector<Vertex> & points) {
        this->points = points;
        send();
    }

    void Mesh::loadFromPoints(std::vector<Vertex> && points) {
        this->points = std::move(points);
        send();
    }

    void Mesh::appendPoints(const std::vector<Vertex> & points) {
        this->points.insert(this->points.end(), points.begin(), points.end());
    }

    void Mesh::send() {
        VBO::loadFromPoints(points);
    }
}

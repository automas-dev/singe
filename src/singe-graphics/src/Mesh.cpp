#include "singe/Graphics/Mesh.hpp"

#include "singe/Support/log.hpp"

namespace singe {
    Mesh::Mesh() : vbo(nullptr) {}

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
        Logging::Core->debug("Mesh send");
        if (!vbo) {
            Logging::Core->debug("Creating VBO");
            vbo = std::make_shared<VBO>();
        }
        vbo->loadFromPoints(points);
    }

    void Mesh::draw() const {
        vbo->draw();
    }
}

#include "singe/Graphics/Mesh.hpp"

#include "singe/Support/log.hpp"
using namespace glpp;

namespace singe {
    Mesh::Mesh()
        : array({{
            Attribute {0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0},
            Attribute {1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0},
            Attribute {2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0},
        }}) {}

    Mesh::Mesh(Mesh && other)
        : array(std::move(other.array)),
          elements(std::move(other.elements)),
          lastSize(other.lastSize) {}

    Mesh & Mesh::operator=(Mesh && other) {
        array = std::move(other.array);
        elements = std::move(other.elements);
        lastSize = other.lastSize;
        return *this;
    }

    Mesh::~Mesh() {}

    void Mesh::loadFromPoints(const std::vector<Vertex> & points,
                              Buffer::Usage usage) {
        elements = points;
        send();
    }

    void Mesh::appendPoints(const std::vector<Vertex> & points,
                            Buffer::Usage usage) {
        elements.insert(elements.end(), points.begin(), points.end());
        send();
    }

    void Mesh::send(Buffer::Usage usage) {
        if (elements.size() == lastSize)
            array.bufferSubData(0, 0, elements.size() * sizeof(Vertex),
                                elements.data());
        else
            array.bufferData(0, elements.size() * sizeof(Vertex),
                             elements.data(), usage);
        lastSize = elements.size();
    }

    void Mesh::draw(Buffer::Mode mode) const {
        array.drawElements(mode, elements.size(), GL_FLOAT, 0);
    }
}

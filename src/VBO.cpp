#include "s3e/VBO.hpp"
#include "s3e/log.hpp"

namespace Tom::s3e {
    VBO::VBO() : vao(0), vbo(0), nPoints(0) {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(0);
    }

    VBO::~VBO() {
        if (vao > 0) {
            glDeleteBuffers(1, &vbo);
            glDeleteVertexArrays(1, &vao);
        }
    }

    bool VBO::loadFromPoints(const std::vector<Vertex> & points) {
        return loadFromPoints(&points[0], points.size());
    }

    bool VBO::loadFromPoints(const Vertex *points, size_t n) {
        nPoints = n;

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*nPoints, points, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(6 * sizeof(float)));

        glBindVertexArray(0);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return true;
    }

    void VBO::draw() const {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, nPoints);
        glBindVertexArray(0);
    }
};


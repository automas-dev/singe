#include "s3e/Graphics/VBO.hpp"

#include "s3e/Support/log.hpp"

namespace Tom::s3e {
    VBO::VBO(VBO::Mode mode, VBO::Usage usage)
        : mode(mode), usage(usage), vao(0), vbo(0), nPoints(0) {

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)(6 * sizeof(float)));

        glBindVertexArray(0);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        Logging::Graphics->debug("VBO Created usage={} vao={} vbo={}", usage,
                                 vao, vbo);
    }

    VBO::~VBO() {
        if (vao > 0) {
            glDeleteBuffers(1, &vbo);
            glDeleteVertexArrays(1, &vao);
        }
    }

    VBO::Mode VBO::getMode() {
        return mode;
    }

    void VBO::setMode(VBO::Mode mode) {
        this->mode = mode;
    }

    VBO::Usage VBO::getUsage() {
        return usage;
    }

    void VBO::setUsage(VBO::Usage usage) {
        this->usage = usage;
    }

    void VBO::loadFromPoints(const std::vector<Vertex> & points) {
        Logging::Graphics->debug("VBO loading {} points vao={} vbo={}",
                                 points.size(), vao, vbo);
        nPoints = points.size();

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * nPoints, points.data(), usage);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VBO::draw() const {
        glBindVertexArray(vao);
        glDrawArrays(mode, 0, nPoints);
        glBindVertexArray(0);
    }
};

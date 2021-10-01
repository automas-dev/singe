#include "InstanceVBO.hpp"

#include <s3e/Support/log.hpp>

namespace Tom::s3e {
    InstanceVBO::InstanceVBO(InstanceVBO::Mode mode, InstanceVBO::Usage usage)
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

        Logging::Graphics->debug("InstanceVBO Created mode={} usage={} vao={} vbo={}",
                                 mode, usage, vao, vbo);
    }

    InstanceVBO::~InstanceVBO() {
        Logging::Graphics->debug("Deleting InstanceVBO vao={} vbo={}", vao, vbo);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    GLuint InstanceVBO::getVBO() const {
        return vbo;
    }

    GLuint InstanceVBO::getVAO() const {
        return vao;
    }

    size_t InstanceVBO::size() const {
        return nPoints;
    }

    InstanceVBO::Mode InstanceVBO::getMode() const {
        return mode;
    }

    void InstanceVBO::setMode(InstanceVBO::Mode mode) {
        Logging::Graphics->debug(
            "Changing mode for InstanceVBO vao={} vbo={} to {}", vao, vbo, mode);
        this->mode = mode;
    }

    InstanceVBO::Usage InstanceVBO::getUsage() const {
        return usage;
    }

    void InstanceVBO::setUsage(InstanceVBO::Usage usage) {
        Logging::Graphics->debug(
            "Changing usage for InstanceVBO vao={} vbo={} to {}", vao, vbo, usage);
        this->usage = usage;
    }

    void InstanceVBO::loadFromPoints(const std::vector<Vertex> & points) {
        Logging::Graphics->debug("InstanceVBO loading {} points vao={} vbo={}",
                                 points.size(), vao, vbo);
        nPoints = points.size();

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * nPoints, points.data(),
                     usage);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void InstanceVBO::draw() const {
        glBindVertexArray(vao);
        glDrawArrays(mode, 0, nPoints);
        glBindVertexArray(0);
    }
}

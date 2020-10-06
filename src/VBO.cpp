#include "VBO.hpp"
#include <iostream>

namespace game {

    static GLuint gen_vbo(const std::vector<Vertex> & points) {
        GLuint VertexVBOID;

        // TODO: Handle errors for each gl call in gen_vbo, return -1 on error

        glGenBuffers(1, &VertexVBOID);
        glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*points.size(), &points[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(6 * sizeof(float)));

        // Get size parameter.
        int32_t bsize = 0;
        glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bsize);
        if (bsize == 0) {
            std::cerr << "vbo has no data" << std::endl;
        }

        return VertexVBOID;
    }

    VBO::VBO() : vao(0), vbo(0), hasBuffer(false), nPoints(0) { }

    VBO::VBO(const std::vector<Vertex> & points) : VBO() {
        loadPoints(points);
    }

    VBO::~VBO() {
        clearPoints();
    }

    void VBO::loadPoints(const std::vector<Vertex> & points) {
        clearPoints();
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        vbo = gen_vbo(points);
        glBindVertexArray(0);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        hasBuffer = true;
        nPoints = points.size();
    }

    void VBO::clearPoints() {
        if (hasBuffer) {
            glDeleteBuffers(1, &vbo);
            glDeleteVertexArrays(1, &vao);
            // TODO: Handle error from glDeleteBuffers
            hasBuffer = false;
            vao = 0;
            vbo = 0;
        }
    }

    void VBO::draw() const {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, nPoints);
        glBindVertexArray(0);

        // glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // glEnableVertexAttribArray(0);
        // glEnableVertexAttribArray(1);
        // glEnableVertexAttribArray(2);

        // glDrawElements(GL_TRIANGLE_FAN, nPoints, GL_UNSIGNED_BYTE, 0);

        // glDisableVertexAttribArray(0);
        // glDisableVertexAttribArray(1);
        // glDisableVertexAttribArray(2);
        // glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    VBO::Ptr VBO::create(const std::vector<Vertex> & points) {
        auto vbo = std::make_shared<VBO>(points);
        return vbo;
    }
};
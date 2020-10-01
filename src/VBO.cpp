#include "VBO.hpp"

namespace game {

    static GLuint gen_vbo(const std::vector<Vertex> &points) {
        GLuint VertexVBOID;

        // TODO: Handle errors for each gl call in gen_vbo, return -1 on error

        glGenBuffers(1, &VertexVBOID);
        glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*points.size(), &points[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8, (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8, (void *)3);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8, (void *)6);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        return VertexVBOID;
    }

    VBO::VBO() : vbo(0), hasBuffer(false), nPoints(0) { }
        
    VBO::VBO(const std::vector<Vertex> &points) : VBO() {
        loadPoints(points);
    }

    VBO::~VBO() {
        clearPoints();
    }

    void VBO::loadPoints(const std::vector<Vertex> &points) {
        clearPoints();
        vbo = gen_vbo(points);
        hasBuffer = true;
        nPoints = points.size();
    }

    void VBO::clearPoints() {
        if (hasBuffer) {
            glDeleteBuffers(1, &vbo);
            // TODO: Handle error from glDeleteBuffers
            hasBuffer = false;
        }
    }

    void VBO::draw() const {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        
        glDrawElements(GL_TRIANGLES, nPoints, GL_UNSIGNED_BYTE, 0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    VBO::Ptr VBO::create(const std::vector<Vertex> &points) {
        auto vbo = std::make_shared<VBO>(points);
        return vbo;
    }
};
#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <cstddef>

namespace game {

    struct Vertex {
        float x, y, z;
        float nx, ny, nz;
        float s0, t0;
    };

    GLuint gen_vbo(Vertex *vertices, size_t len) {
        GLuint VertexVBOID;

        glGenBuffers(1, &VertexVBOID);
        glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*len, &vertices[0].x, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8, (void*)3);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8, (void*)6);

        return VertexVBOID;
    }

    class VBO {
        GLuint vbo;

    public:
        VBO(Vertex *data, size_t n) {
            vbo = gen_vbo(data, n);
        }

        ~VBO(void) {
            glDeleteBuffers(1, &vbo);
        }
    };

    class VAO {
        GLuint vao;

    public:
        VAO() {
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
        }

        ~VAO(void) {
        }
    };
}

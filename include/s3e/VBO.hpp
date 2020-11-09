#pragma once

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <cstddef>

namespace Tom::s3e {

    struct Vertex {
        glm::vec3 pos;
        glm::vec3 norm;
        glm::vec2 uv;

        Vertex(void) {}
        Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 uv)
            : pos(pos), norm(norm), uv(uv) { }
        ~Vertex() {}
    };

    class VBO {
        GLuint vao;
        GLuint vbo;
        bool hasBuffer;
        size_t nPoints;

    public:
        typedef std::shared_ptr<VBO> Ptr;
        typedef std::shared_ptr<const VBO> ConstPtr;

        VBO(void);
        VBO(const std::vector<Vertex> & points);
        virtual ~VBO();

        void loadPoints(const std::vector<Vertex> & points);

        void clearPoints(void);

        void draw(void) const;

        static Ptr create(const std::vector<Vertex> & points);
    };

    // class VAO {
    //     GLuint vao;

    // public:
    //     VAO() {
    //         glGenVertexArrays(1, &vao);
    //         glBindVertexArray(vao);
    //     }

    //     ~VAO(void) {
    //     }
    // };
}

#pragma once

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <cstddef>

namespace game {

    struct Vertex {
        glm::vec3 pos;
        glm::vec3 norm;
        glm::vec2 uv;

        Vertex(void) {}
        Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 uv)
            : pos(pos), norm(norm), uv(uv) { }
        ~Vertex() {}
    };

    class VBO : public std::enable_shared_from_this<VBO> {
        GLuint vbo;
        bool hasBuffer;

    public:
        typedef std::shared_ptr<VBO> Ptr;
        typedef std::shared_ptr<const VBO> ConstPtr;

        VBO();
        VBO(const std::vector<Vertex> & points);
        virtual ~VBO();

        void loadPoints(const std::vector<Vertex> & points);

        void clearPoints(void);

        void draw(void) const;

        static Ptr create(const std::vector<Vertex> & points);
    };

    // class VAO : public std::enable_shared_from_this<VAO> {
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

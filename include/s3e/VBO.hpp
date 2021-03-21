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

        Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 uv) : pos(pos), norm(norm), uv(uv) { }

        Vertex(const Vertex & other) {
            pos = other.pos;
            norm = other.norm;
            uv = other.uv;
        }

        Vertex(Vertex && other) {
            std::swap(pos, other.pos);
            std::swap(norm, other.norm);
            std::swap(uv, other.uv);
        }

        Vertex & operator=(const Vertex & other) {
            if (this == &other)
                return *this;

            pos = other.pos;
            norm = other.norm;
            uv = other.uv;
            return *this;
        }

        Vertex operator+(const Vertex & other) const {
            return Vertex(pos + other.pos, norm + other.norm, uv + other.uv);
        }

        Vertex operator+(const glm::vec3 & offset) const {
            return Vertex(pos + offset, norm, uv);
        }

        Vertex operator-(const Vertex & other) const {
            return Vertex(pos - other.pos, norm - other.norm, uv - other.uv);
        }

        Vertex & operator+=(const Vertex & other) {
            pos += other.pos;
            norm += other.norm;
            uv += other.uv;
            return *this;
        }

        Vertex & operator-=(const Vertex & other) {
            pos -= other.pos;
            norm -= other.norm;
            uv -= other.uv;
            return *this;
        }
    };

    class VBO {
        GLuint vao;
        GLuint vbo;
        size_t nPoints;

    public:
        typedef std::shared_ptr<VBO> Ptr;
        typedef std::shared_ptr<const VBO> ConstPtr;

        VBO(void);
        virtual ~VBO();

        bool loadFromPoints(const std::vector<Vertex> & points);

        bool loadFromPoints(const Vertex *points, size_t n);

        void draw(void) const;
    };
}


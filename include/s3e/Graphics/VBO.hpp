#pragma once

#include <GL/glew.h>

#include <SFML/OpenGL.hpp>
#include <cstddef>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "s3e/Support/log.hpp"

namespace Tom::s3e {

    struct Vertex {
        glm::vec3 pos;
        glm::vec3 norm;
        glm::vec2 uv;

        Vertex(void) : pos(0), norm(0), uv(0) {}

        Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 uv)
            : pos(pos), norm(norm), uv(uv) {}

        Vertex(const Vertex & other) = default;

        Vertex(Vertex && other) = default;

        Vertex & operator=(const Vertex & other) = default;

        Vertex & operator=(Vertex && other) = default;

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

    /**
     * Manages a single vertex buffered object and vertex array object.
     */
    class VBO {
    public:
        /**
         * OpenGL Draw mode.
         */
        enum Mode {
            Lines = GL_LINES,
            LineStrip = GL_LINE_STRIP,
            LineLoop = GL_LINE_LOOP,

            Triangles = GL_TRIANGLES,
            TriangleStrip = GL_TRIANGLE_STRIP,
            TriangleFan = GL_TRIANGLE_FAN,
        };

        /**
         * OpenGL buffer usage.
         */
        enum Usage {
            /// Data will be buffered once and used multiple times
            Static = GL_STATIC_DRAW,
            /// Data will be buffered and used a few times
            Stream = GL_STREAM_DRAW,
            /// Data will be buffered and used once
            Dynamic = GL_DYNAMIC_DRAW,
        };

    private:
        GLuint vao;
        GLuint vbo;
        Mode mode;
        Usage usage;
        size_t nPoints;

    public:
        typedef std::shared_ptr<VBO> Ptr;
        typedef std::shared_ptr<const VBO> ConstPtr;

        /**
         * Create a new VBO with mode and usage.
         *
         * @param mode the OpenGL draw mode
         * @param usage the OpenGL buffer usage
         */
        VBO(Mode mode = Mode::Triangles, Usage usage = Usage::Static);

        /**
         * Free OpenGL buffers.
         */
        virtual ~VBO();

        /**
         * Get the OpenGL draw mode.
         *
         * @return the VBO::Mode
         */
        Mode getMode();

        /**
         * Set the OpenGL draw mode.
         *
         * @param mode the new mode
         */
        void setMode(Mode mode);

        /**
         * Get the OpenGL buffer usage.
         *
         * @return the VBO::Usage
         */
        Usage getUsage();

        /**
         * Set the OpenGL buffer usage.
         *
         * @param usage the new usage
         */
        void setUsage(Usage usage);

        /**
         * Load buffer with data from points.
         *
         * @param points the data to send to the buffer
         */
        void loadFromPoints(const std::vector<Vertex> & points);

        /**
         * Load buffer with data from points.
         *
         * @param points the data to send to the buffer
         * @param n the numebr of points
         */
        void loadFromPoints(const Vertex * points, size_t n);

        /**
         * Bind the vao, then draw, then unbind the vao.
         */
        void draw() const;
    };

    struct Mesh : VBO {
        std::vector<Vertex> points;

        using VBO::Mode;
        using VBO::Usage;

        typedef std::shared_ptr<Mesh> Ptr;
        typedef std::shared_ptr<const Mesh> ConstPtr;

        using VBO::VBO;
        virtual ~Mesh() {}

        using VBO::getMode;
        using VBO::setMode;
        using VBO::getUsage;
        using VBO::setUsage;
        using VBO::draw;

        /**
         * Store points for future use.
         *
         * @param points the points to store
         */
        void loadFromPoints(const std::vector<Vertex> & points) {
            this->points = points;
            send();
        }

        /**
         * Load buffer with data from points.
         *
         * @param points the data to send to the buffer
         */
        void loadFromPoints(std::vector<Vertex> && points) {
            this->points = std::move(points);
            send();
        }

        /**
         * Append points to this mesh.
         * 
         * Note: this method does not call send().
         * 
         * @param points the points to append
         */
        void appendPoints(const std::vector<Vertex> & points) {
            this->points.insert(this->points.end(), points.begin(), points.end());
            Logging::Graphics->trace("Mesh appending {} points, size is {}", points.size(), this->points.size());
        }

        /**
         * Send points to the OpenGL buffer.
         */
        void send() {
            VBO::loadFromPoints(points);
        }
    };
}

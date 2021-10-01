#pragma once

#include <s3e/Graphics/VBO.hpp>

namespace Tom::s3e {

    /**
     * Describes a single vertex attribute.
     */
    struct VertexAttribute {
        GLuint index;
        GLint size;
        bool type;
        bool normalized;
        GLsizei stride;
        const void * pointer;
    };

    /**
     * Manages a single vertex buffered object and vertex array object.
     */
    class InstanceVBO {
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
        using Ptr = std::shared_ptr<VBO>;
        using ConstPtr = std::shared_ptr<const VBO>;

        /**
         * Create a new VBO with mode and usage.
         *
         * @param mode the OpenGL draw mode
         * @param usage the OpenGL buffer usage
         */
        InstanceVBO(Mode mode = Mode::Triangles, Usage usage = Usage::Static);

        /**
         * Free OpenGL buffers.
         */
        virtual ~InstanceVBO();

        /**
         * Get the OpenGL vbo id.
         *
         * @return the vbo id
         */
        GLuint getVBO() const;

        /**
         * Get the OpenGL vao id.
         *
         * @return the vao id
         */
        GLuint getVAO() const;

        /**
         * Get the number of points.
         *
         * @return the number of points
         */
        size_t size() const;

        /**
         * Get the OpenGL draw mode.
         *
         * @return the VBO::Mode
         */
        Mode getMode() const;

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
        Usage getUsage() const;

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
         * Bind the vao, then draw, then unbind the vao.
         */
        void draw() const;
    };
}
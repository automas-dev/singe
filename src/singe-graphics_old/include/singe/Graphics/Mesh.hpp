#pragma once

#include <glpp/Buffer.hpp>
#include <glpp/extra/Buffer.hpp>
#include <memory>
#include <vector>

namespace singe {
    using glpp::Buffer;
    using glpp::BufferArray;
    using glpp::extra::Vertex;

    /**
     * Derived class of VBO which keeps a CPU side copy of points.
     */
    class Mesh {
        BufferArray array;
        size_t lastSize;

    public:
        std::vector<Vertex> elements;

        Mesh();

        Mesh(const std::vector<Vertex> & elements);

        Mesh(std::vector<Vertex> && elements);

        Mesh(Mesh && other);

        Mesh & operator=(Mesh && other);

        Mesh(const Mesh &) = delete;
        Mesh & operator=(const Mesh &) = delete;

        ~Mesh();

        /**
         * Store points for future use.
         *
         * Note: this method does call send().
         *
         * @param points the points to store
         */
        void loadFromPoints(const std::vector<Vertex> & points,
                            Buffer::Usage usage = Buffer::Static);

        /**
         * Append points to this mesh.
         *
         * Note: this method does call send().
         *
         * @param points the points to append
         */
        void appendPoints(const std::vector<Vertex> & points,
                          Buffer::Usage usage = Buffer::Static);

        /**
         * Buffer points in OpenGL memory. This must be called before draw.
         *
         * If the size of elements has not changed, use glBufferSubData.
         */
        void send(Buffer::Usage usage = Buffer::Static);

        /**
         * Call draw on vbo.
         */
        void draw(Buffer::Mode mode = Buffer::Triangles) const;
    };
}

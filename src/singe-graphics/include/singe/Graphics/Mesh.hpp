#pragma once

#include <glpp/Buffer.hpp>
#include <glpp/extra/Vertex.hpp>
#include <memory>
#include <vector>

#include "Material.hpp"
#include "Mesh.hpp"
#include "RenderState.hpp"

namespace singe {
    using std::shared_ptr;
    using std::vector;

    using glpp::Buffer;
    using glpp::extra::Vertex;
    using glpp::extra::VertexBufferArray;

    /**
     * VertexArrayBuffer with optional Material.
     *
     * Remember to call Mesh::update() after making changes to points. This will
     * buffer the mesh points into the vertex buffer.
     */
    class Mesh {
        VertexBufferArray array;

    public:
        vector<Vertex> points;
        shared_ptr<Material> material;

        /**
         * Create an empty Mesh. This will do nothing until points are added to
         * mesh and Mesh::update() is called.
         */
        Mesh();

        /**
         * Create a Mesh by copying points. This constructor will call
         * Mesh::update().
         */
        Mesh(const vector<Vertex> & points);

        /**
         * Create a Mesh by moving points. This constructor will call
         * Mesh::update().
         */
        Mesh(vector<Vertex> && points);

        /// @brief  Move constructor
        /// @param other Other Mesh to move fields from
        Mesh(Mesh && other);

        /// @brief Move operator
        /// @param other Other Mesh to move fields from
        /// @return This Mesh
        Mesh & operator=(Mesh && other);

        Mesh(const Mesh &) = delete;
        Mesh & operator=(const Mesh &) = delete;

        /// @brief Make this a virtual class for derivation.
        virtual ~Mesh();

        /**
         * Buffer points into the vertex buffer.
         *
         * This method must be called after any changes to points.
         */
        void update(Buffer::Usage usage = Buffer::Static);

        /**
         * Draw the vertex buffer.
         *
         * You must call Mesh::update() to buffer any changes to points before
         * drawing.
         */
        void draw(RenderState & state) const;
    };
}

#pragma once

#include <glpp/Buffer.hpp>
#include <glpp/extra/Vertex.hpp>
#include <memory>

#include "Mesh.hpp"
#include "RenderState.hpp"

namespace singe {
    using std::shared_ptr;

    using glpp::Buffer;
    using glpp::extra::VertexBufferArray;

    /**
     * VertexArrayBuffer with optional pointer to Mesh.
     *
     * If mesh is null, this class will do nothing.
     *
     * Remember to call Geometry::update() after making changes to mesh. This
     * will buffer the mesh points into the vertex buffer.
     */
    class Geometry {
        VertexBufferArray array;
        size_t n;

    public:
        /// @brief Optional mesh with vertex points
        shared_ptr<Mesh> mesh;

        /**
         * Create a Geometry with no mesh. This will do nothing until a mesh is
         * attached and Geometry::update() is called.
         */
        Geometry();

        /**
         * Create a Geometry with a mesh. This constructor will call
         * Geometry::update() if mesh is not null.
         */
        Geometry(const shared_ptr<Mesh> & mesh);

        /// @brief  Move constructor
        /// @param other Other Geometry to move fields from
        Geometry(Geometry && other);

        /// @brief Move operator
        /// @param other Other Geometry to move fields from
        /// @return This Geometry
        Geometry & operator=(Geometry && other);

        Geometry(const Geometry &) = delete;
        Geometry & operator=(const Geometry &) = delete;

        ~Geometry();

        /**
         * Buffer mesh points into the vertex buffer.
         *
         * If mesh is null, this method will do nothing. This method must be
         * called after any changes to mesh.
         */
        void update(Buffer::Usage usage = Buffer::Static);

        /**
         * Draw the vertex buffer.
         *
         * If mesh is null, this method will do nothing. You must call
         * Geometry::update() to buffer any points or changes in mesh before
         * drawing.
         */
        void draw(RenderState & state) const;
    };
}

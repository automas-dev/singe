#pragma once

#include <glpp/Buffer.hpp>
#include <glpp/extra/Vertex.hpp>
#include <memory>
#include <vector>

#include "Material.hpp"
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
     * Remember to call Model::update() after making changes to points. This
     * will buffer the mesh points into the vertex buffer.
     */
    class Model {
        VertexBufferArray array;

    public:
        vector<Vertex> points;
        shared_ptr<Material> material;
        Transform transform;

        /**
         * Create an empty Model. This will do nothing until points are added to
         * mesh and Model::update() is called.
         */
        Model();

        /**
         * Create a Model by copying points. This constructor will call
         * Model::update().
         *
         * @param points the Vertex points for this mesh
         */
        Model(const vector<Vertex> & points);

        /**
         * Create a Model by moving points. This constructor will call
         * Model::update().
         *
         * @param points the Vertex points for this mesh
         */
        Model(vector<Vertex> && points);

        /// @brief  Move constructor
        /// @param other Other Model to move fields from
        Model(Model && other);

        /// @brief Move operator
        /// @param other Other Model to move fields from
        /// @return This Model
        Model & operator=(Model && other);

        Model(const Model &) = delete;
        Model & operator=(const Model &) = delete;

        /// @brief Make this a virtual class for derivation.
        virtual ~Model();

        /**
         * Buffer points into the vertex buffer.
         *
         * This method must be called after any changes to points.
         *
         * @param usage glpp::Buffer usage hint
         */
        void update(Buffer::Usage usage = Buffer::Static);

        /**
         * Draw the vertex buffer.
         *
         * You must call Model::update() to buffer any changes to points before
         * drawing.
         *
         * @param state the parent state with transform for shader's mvp uniform
         */
        void draw(RenderState state) const;
    };
}

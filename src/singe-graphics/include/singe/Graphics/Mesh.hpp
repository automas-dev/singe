#pragma once

#include <glpp/Buffer.hpp>
#include <memory>
#include <vector>

namespace singe {
    /**
     * Derived class of VBO which keeps a CPU side copy of points.
     */
    struct Mesh {
        glpp::TextureVertexBuffer buffer;

        using Ptr = std::shared_ptr<Mesh>;
        using ConstPtr = std::shared_ptr<const Mesh>;

        Mesh();
        virtual ~Mesh();

        /**
         * Store points for future use.
         *
         * @param points the points to store
         */
        void loadFromPoints(const std::vector<glpp::Vertex> & points);

        /**
         * Append points to this mesh.
         *
         * Note: this method does not call send().
         *
         * @param points the points to append
         */
        void appendPoints(const std::vector<glpp::Vertex> & points);

        /**
         * Call draw on vbo.
         */
        virtual void draw(glpp::Buffer::Mode mode) const;
    };
}

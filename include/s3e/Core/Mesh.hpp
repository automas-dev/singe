#pragma once

#include <memory>
#include <vector>

#include "s3e/Graphics/VBO.hpp"

namespace Tom::s3e {
    /**
     * Derived class of VBO which keeps a CPU side copy of points.
     */
    struct Mesh : VBO {
        std::vector<Vertex> points;

        using VBO::Mode;
        using VBO::Usage;

        using Ptr = std::shared_ptr<Mesh>;
        using ConstPtr = std::shared_ptr<const Mesh>;

        using VBO::VBO;
        virtual ~Mesh();

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
        void loadFromPoints(const std::vector<Vertex> & points);

        /**
         * Load buffer with data from points.
         *
         * @param points the data to send to the buffer
         */
        void loadFromPoints(std::vector<Vertex> && points);

        /**
         * Append points to this mesh.
         *
         * Note: this method does not call send().
         *
         * @param points the points to append
         */
        void appendPoints(const std::vector<Vertex> & points);

        /**
         * Send points to the OpenGL buffer.
         */
        void send();
    };
}

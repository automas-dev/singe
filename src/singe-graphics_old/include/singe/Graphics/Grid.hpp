#pragma once

#include <glm/glm.hpp>
#include <glpp/Buffer.hpp>
#include <glpp/Shader.hpp>

#include "singe/Graphics/RenderState.hpp"

namespace singe {
    using glpp::Uniform;
    using glpp::Shader;
    using glpp::BufferArray;
    using glm::vec3;

    class Grid {
        BufferArray buffer;
        int n;
        Shader shader;
        Uniform mvp;

    public:
        Grid(int n, float spacing = 1, const vec3 & color = vec3(1));

        Grid(Grid && other);

        Grid & operator=(Grid && other);

        Grid(const Grid &) = delete;
        Grid & operator=(const Grid &) = delete;

        ~Grid();

        void draw(RenderState state) const;
    };
}
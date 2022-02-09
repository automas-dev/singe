#pragma once

#include <glm/glm.hpp>
#include <glpp/Buffer.hpp>
#include <glpp/Shader.hpp>
#include <memory>

#include "singe/Graphics/RenderState.hpp"

namespace singe {
    using glpp::Shader;
    using glpp::TextureVertexBuffer;
    using std::shared_ptr;

    class Grid : TextureVertexBuffer {
        glm::vec3 color;
        bool fade;
        shared_ptr<Shader> shader;

    public:
        using Ptr = shared_ptr<Grid>;

        Grid(int n,
             float spacing = 1.0,
             const glm::vec3 & color = {1, 1, 1},
             bool fade = false);
        virtual ~Grid();

        void draw(RenderState state) const;
    };
}
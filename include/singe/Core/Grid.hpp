#pragma once

#include "singe/Core/RenderState.hpp"
#include "singe/Graphics/Shader.hpp"
#include "singe/Graphics/VBO.hpp"

namespace singe {
    class Grid : VBO {
        glm::vec3 color;
        bool fade;
        Shader::Ptr shader;

    public:
        using Ptr = std::shared_ptr<Grid>;

        Grid(int n,
             float spacing = 1.0,
             const glm::vec3 & color = {1, 1, 1},
             bool fade = false);
        virtual ~Grid();

        void draw(RenderState state) const;
    };
}
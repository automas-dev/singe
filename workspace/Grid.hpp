#pragma once

#include <singe/Graphics/VBO.hpp>
#include <singe/Graphics/Shader.hpp>
#include <singe/Core/RenderState.hpp>

namespace singe {
    class Grid : public VBO {
        glm::vec3 color;
        static Shader::Ptr shader;

    public:
        using Ptr = std::shared_ptr<Grid>;

        Grid(int n, float spacing = 1.0, const glm::vec3 & color = {1, 1, 1});
        virtual ~Grid();

        // void draw(RenderState state) const;
    };
}
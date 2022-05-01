#pragma once

#include <glm/glm.hpp>
#include <glpp/Shader.hpp>

namespace singe {
    struct RenderState {
        const glpp::Shader & shader;
        glm::mat4 transform;

        RenderState(const glpp::Shader & shader, glm::mat4 & transform);

        RenderState(const RenderState & other) = default;

        RenderState(RenderState && other) = default;

        RenderState & operator=(const RenderState & other) = default;

        RenderState & operator=(RenderState && other) = default;

        void sendMVP() const;
    };
}
#include "singe/Graphics/Grid.hpp"

#include <string_view>

static const std::string_view gridVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 mvp;
out vec3 FragPos;
void main() {
    gl_Position = mvp * vec4(aPos, 1.0);
    FragPos = aPos;
})";

static const std::string_view gridFragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 color;
uniform bool fade;
uniform vec3 camera;
in vec3 FragPos;
void main() {
    float a = 1.0;
    if (fade) {
        float h = abs(camera.y);
        float d = distance(FragPos, vec3(camera.x, 0.0, camera.z));
        float start = -h/tan(radians(-20));
        float end = -h/tan(radians(-5));
        a = (d-end) / (start-end);
        a = clamp(a, 0, 1);
    }
    FragColor = vec4(color, a);
})";

namespace singe {
    using glpp::Vertex;
    using std::make_shared;

    Grid::Grid(int n, float spacing, const glm::vec3 & color, bool fade)
        : TextureVertexBuffer(Buffer::Static), color(color), fade(fade) {
        std::vector<Vertex> lines;
        for (int i = -n; i <= n; i++) {
            // x line
            lines.push_back(Vertex({-n, 0, i}, {0, 0, 0}, {0, 0}));
            lines.push_back(Vertex({n, 0, i}, {0, 0, 0}, {0, 0}));

            // z line
            lines.push_back(Vertex({i, 0, -n}, {0, 0, 0}, {0, 0}));
            lines.push_back(Vertex({i, 0, n}, {0, 0, 0}, {0, 0}));
        }
        loadFromPoints(lines);

        shader = make_shared<glpp::Shader>(gridVertexShaderSource,
                                           gridFragmentShaderSource);
    }

    Grid::~Grid() {}

    void Grid::draw(RenderState state) const {
        shader->bind();
        shader->setMat4("mvp", state.transform);
        shader->setVec3("color", color);
        shader->setInt("fade", fade);
        shader->setVec3("camera", state.camera->getPosition());
        Buffer::draw(Buffer::Lines);
    }
}
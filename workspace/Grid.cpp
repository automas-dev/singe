#include "Grid.hpp"

static const std ::string gridVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 mvp;
void main() {
    gl_Position = mvp * vec4(aPos, 1.0);
})";

static const std ::string gridFragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 camera;
void main() {
    float h = abs(camera.y);
    float d = distance(FragPos, vec3(camera.x, 0.0, camera.z));
    float start = -h/tan(radians(-20));
    float end = -h/tan(radians(-5));
    float a = (d-end) / (start-end);
    a = clamp(a, 0, 1);
    if (FragPos.x == 0) {
        FragColor = vec4(1.0, 0.0, 0.0, a);
    }
    else if (FragPos.z == 0) {
        FragColor = vec4(0.0, 0.0, 1.0, a);
    }
    else {
        FragColor = vec4(0.4, 0.4, 0.4, a);
    }
})";

namespace singe {
    Grid::Grid(int n, float spacing, const glm::vec3 & color) : VBO(VBO::Lines, VBO::Static),
    color(color) {
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

        // if (!shader) {
        //     shader = std::make_shared<Shader>();
        //     shader->loadFromSource(gridVertexShaderSource, gridFragmentShaderSource);
        // }
    }

    Grid::~Grid() {}

    // void Grid::draw(RenderState state) const {
    //     shader->bind();
    //     shader->setMat4("mvp", mvp);
    //     VBO::draw();
    // }
}
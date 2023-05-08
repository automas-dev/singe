#include "singe/Graphics/Grid.hpp"

#include <memory>
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
// uniform bool fade;
// uniform vec3 camera;
in vec3 FragPos;
void main() {
    float a = 1.0;
    // if (fade) {
    //     float h = abs(camera.y);
    //     float d = distance(FragPos, vec3(camera.x, 0.0, camera.z));
    //     float start = -h/tan(radians(-20));
    //     float end = -h/tan(radians(-5));
    //     a = (d-end) / (start-end);
    //     a = clamp(a, 0, 1);
    // }
    FragColor = vec4(color, a);
})";

namespace singe {
    using std::move;
    using std::vector;
    using glpp::Attribute;
    using glpp::Buffer;

    Grid::Grid(int n, float spacing, const vec3 & color)
        : buffer({{Attribute {0, 3, GL_FLOAT, false, 3 * sizeof(float), 0}}}),
          n(0),
          shader(gridVertexShaderSource, gridFragmentShaderSource),
          mvp(shader.uniform("mvp")) {

        vector<vec3> lines;
        for (int i = -n; i <= n; i++) {
            // x line
            lines.push_back({-n, 0, i});
            lines.push_back({n, 0, i});

            // z line
            lines.push_back({i, 0, -n});
            lines.push_back({i, 0, n});
        }

        buffer.bufferData(0, 3 * lines.size() * sizeof(float), lines.data());
        n = 3 * lines.size();

        shader.uniform("color").setVec3(color);
    }

    Grid::Grid(Grid && other)
        : buffer(move(other.buffer)),
          n(other.n),
          shader(move(other.shader)),
          mvp(move(other.mvp)) {}

    Grid & Grid::operator=(Grid && other) {
        buffer = move(other.buffer);
        n = other.n;
        shader = move(other.shader);
        mvp = move(other.mvp);
        return *this;
    }

    Grid::~Grid() {}

    void Grid::draw(RenderState state) const {
        shader.bind();
        mvp.setMat4(state.transform);
        buffer.drawArrays(Buffer::Lines, 0, n);
    }
}
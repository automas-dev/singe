#include "Game.hpp"

#include <stdexcept>

static const char * vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
uniform mat4 mvp;
out vec3 color;
void main() {
    gl_Position = mvp * vec4(aPos, 1.0);
    color = aCol;
})";

static const char * fragmentShaderSource = R"(
#version 330 core
in vec3 color;
out vec4 FragColor;
void main() {
    FragColor = vec4(color, 1.0);
})";

Game::Game(Window & window) : GameBase(window) {
    camera.setPosition({5, 2, 5});
    camera.setFov(70);

    // TODO: load here

    model = Model::fromPath("../../workspace/res/model/cube.obj");

    mvp = defaultShader.uniform("mvp");

    // Load models / textures / scenes
    // No fancy render api, just each model can be drawn
    // Maybe add something like pyglet Batch to group rendering

    window.setMouseGrab(true);
}

Game::~Game() {}

void Game::onUpdate(const sf::Time & delta) {
    // TODO: update here
}

inline void setupGl() {
    glClearColor(0.25, 0.25, 0.25, 1.0);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Game::onDraw() const {
    setupGl();

    glm::mat4 vp = camera.projMatrix() * camera.viewMatrix();
    defaultShader.bind();
    mvp.setMat4(vp);

    // TODO: draw here
    // RenderState state(defaultShader, vp);
    // grid->draw(state);
}

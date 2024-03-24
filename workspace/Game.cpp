#include "Game.hpp"

#include <stdexcept>

using std::make_shared;
using glpp::extra::Grid;

static const char * circle_vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aCol;
out vec4 color;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
    color = aCol;
})";

static const char * circle_fragmentShaderSource = R"(
#version 330 core
in vec4 color;
out vec4 FragColor;
void main() {
    // FragColor = color;
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
})";

Game::Game(Window::Ptr & window)
    : GameBase(window),
      res("../../examples/res"),
      shader(res.getMVPShader("shader/default.vert", "shader/default.frag")),
      circle_shader(make_shared<singe::Shader>(glpp::Shader(
          circle_vertexShaderSource, circle_fragmentShaderSource))),
      circle(make_shared<Circle>(0.03, vec4(1.0, 0.0, 0.0, 1.0))) {

    camera.setPosition({5, 2, 5});
    camera.setRotation({0.2, -0.75, 0});

    scene.models = res.loadModel("model/cube.obj");
    scene.models[0]->material->shader = shader;

    scene.grid = make_shared<Grid>(10, vec4(1, 1, 1, 1), true);

    circle->setPos({0.5, 0.9});

    window->setMouseGrab(true);
}

Game::~Game() {}

void Game::onUpdate(const sf::Time & delta) {
    float s = delta.asSeconds();
    scene.models[0]->transform.rotateEuler({0, s * 0.5, 0});

    RenderState tmpState (camera);
    tmpState.pushTransform(scene.transform.toMatrix());
    tmpState.pushTransform(scene.models[0]->transform.toMatrix());
    point = scene.models[0]->points[0].pos;
    auto projPoint = tmpState.getMVP() * vec4(point, 1.0);
    projPoint /= projPoint.w;
    point = vec3(projPoint.x, projPoint.y, 0.0);
    circle->setPos(point);
}

inline void setupGl() {
    glClearColor(0.25, 0.25, 0.25, 1.0);
    glDisable(GL_CULL_FACE);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Game::onDraw() const {
    setupGl();

    RenderState state(camera);
    state.setGridEnable(true);
    scene.draw(state);

    mat4 mvp = state.getMVP();
    auto screen = mvp * glm::vec4(point, 0);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    circle_shader->shader().bind();
    circle->draw();

    glpp::BufferArray::unbind();
}

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

Game::Game(Window & window)
    : GameBase(window),
      res("../../../examples/res"),
      shader(Shader::defaultShader()),
      gridShader(Grid::shader()),
      grid(10, {1, 1, 1, 1}, true),
      showGrid(true),
      wireframe(Fill) {

    camera.setPosition({3, 2, 1});
    camera.setRotation({0, -1, 0});
    camera.setFov(80);

    mvp = shader.uniform("mvp");

    scene.models.emplace_back(res.loadModel("cube_plane.obj"));

    auto & sphereModel =
        scene.models.emplace_back(res.loadModel("sphere.obj"));
    sphereModel.transform.move({1, 2, 3});
    sphereModel.transform.scale({0.1, 0.1, 0.1});

    auto & hallModel = scene.models.emplace_back(res.loadModel("hall.obj"));
    hallModel.transform.move({0, 0, -5});

    // Load models / textures / scenes
    // No fancy render api, just each model can be drawn
    // Maybe add something like pyglet Batch to group rendering

    window.setMouseGrab(true);

    Logging::Game->info("Keyboard:");
    Logging::Game->info("W - Forward");
    Logging::Game->info("S - Backward");
    Logging::Game->info("A - Left");
    Logging::Game->info("D - Right");
    Logging::Game->info("E - Up");
    Logging::Game->info("Q - Down");
    Logging::Game->info("1 - Point");
    Logging::Game->info("2 - Line");
    Logging::Game->info("3 - Fill");
    Logging::Game->info("G - Toggle Grid");
}

Game::~Game() {}

void Game::onKeyPressed(const sf::Event::KeyEvent & event) {
    switch (event.code) {
        case sf::Keyboard::Num1:
            wireframe = Point;
            break;
        case sf::Keyboard::Num2:
            wireframe = Line;
            break;
        case sf::Keyboard::Num3:
            wireframe = Fill;
            break;
        case sf::Keyboard::G:
            showGrid = !showGrid;
            break;
        default:
            break;
    }
    GameBase::onKeyPressed(event);
}

void Game::onUpdate(const sf::Time & delta) {
    float s = delta.asSeconds();
}

inline void setupGl() {
    glClearColor(0.25, 0.25, 0.25, 1.0);
    glDisable(GL_CULL_FACE);
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

    glPolygonMode(GL_FRONT_AND_BACK, wireframe);
    glPointSize(2.0);

    glm::mat4 vp = camera.projMatrix() * camera.viewMatrix();
    shader.bind();
    mvp.setMat4(vp);

    RenderState state(vp, shader);
    scene.draw(state);

    if (showGrid) {
        gridShader.bind();
        Uniform gridMvp = gridShader.uniform("mvp");
        gridMvp.setMat4(vp);
        grid.draw();
    }

    glpp::BufferArray::unbind();

    glPolygonMode(GL_FRONT_AND_BACK, Fill);
}

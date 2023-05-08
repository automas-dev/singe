#include "Game.hpp"

#include <stdexcept>

Game::Game(Window & window)
    : GameBase(window),
      res("../../../examples/res"),
      shader(res.getShader("default")),
      gridShader(res.getShader("grid")),
      grid(10, {1, 1, 1, 1}, true),
      showGrid(true),
      wireframe(Fill) {

    camera.setPosition({5, 2, 5});
    camera.setRotation({0, -1, 0});
    camera.setFov(70);

    shared_ptr<Mesh> model;

    auto & objectScene = scene.children.emplace_back(std::make_shared<Scene>());
    objectScene->transform.move({0, 2, 0});

    model = objectScene->models.emplace_back(res.loadModel("sphere.obj"));
    model->material->shader = shader;

    model = scene.models.emplace_back(res.loadModel("plane.obj"));
    model->material->shader = shader;

    auto otherScene = scene.children.emplace_back(std::make_shared<Scene>());

    model = otherScene->models.emplace_back(res.loadModel("fountain.obj"));
    model->transform.move({0, 0, 3});
    model->material->shader = shader;

    model = otherScene->models.emplace_back(res.loadModel("Human.obj"));
    model->transform.move({3, 0, 0});
    model->material->shader = shader;

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
    scene.children[0]->transform.rotateEuler({s, s * 0.2, 0});
    otherScene->transform.rotateEuler({0, s * 0.1, 0});
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
    RenderState state(vp);
    scene.draw(state);

    if (showGrid) {
        gridShader->bind();
        gridShader->mvp().setMat4(vp);
        grid.draw();
    }

    glpp::BufferArray::unbind();

    glPolygonMode(GL_FRONT_AND_BACK, Fill);
}

#include "Game.hpp"

#include <stdexcept>

Game::Game(Window & window)
    : GameBase(window),
      res("../../../examples/res"),
      shader(res.getMVPShader("default")),
      grid(10, {1, 1, 1, 1}, true),
      showGrid(true),
      wireframe(Fill) {

    camera.setPosition({5, 2, 5});
    camera.setRotation({0, -1, 0});
    camera.setFov(70);

    shared_ptr<Scene> modelScene;

    auto objectScene = scene.addChild();
    objectScene->transform.move({0, 2, 0});

    modelScene = objectScene->addChild();
    modelScene->models = res.loadModel("sphere.obj");

    modelScene = objectScene->addChild();
    modelScene->models = res.loadModel("plane.obj");

    otherScene = scene.addChild();

    modelScene = otherScene->addChild();
    modelScene->models = res.loadModel("fountain.obj");
    modelScene->transform.move({0, 0, 3});

    modelScene = otherScene->addChild();
    modelScene->models = res.loadModel("Human.obj");
    modelScene->transform.move({3, 0, 0});

    for (auto & s : scene.children) {
        for (auto & ss : s->children) {
            for (auto & m : ss->models) m->material->shader = shader;
        }
    }

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
    scene.children[0]->children[0]->transform.rotateEuler({s, s * 0.2, 0});
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

    RenderState state(camera);
    scene.draw(state);

    if (showGrid) {
        grid.draw(state.getMVP());
    }

    glpp::BufferArray::unbind();

    glPolygonMode(GL_FRONT_AND_BACK, Fill);
}

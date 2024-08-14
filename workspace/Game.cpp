#include "Game.hpp"

#include <tinyxml2.h>

#include <stdexcept>
#include <string>

#include "FastScene.hpp"

using std::make_shared;
using namespace std;
using glpp::extra::Grid;

void Game::parseScene() {
    tinyxml2::XMLDocument doc;
    doc.LoadFile("../../workspace/res/scene.xml");
    if (doc.ErrorID()) {
        Logging::Game->error("Failed to open scene xml");
        return;
    }

    auto ns = fast::parseScene(&doc);
    scene.grid = ns->grid;
}

Game::Game(Window::Ptr & window)
    : GameBase(window), res("../../examples/res") {

    drawGrid = true;

    parseScene();

    shader = res.getMVPShader("shader/default.vert", "shader/default.frag");

    camera.setPosition({3, 2, 3});
    camera.setRotation({0.2, -0.75, 0});

    // scene.grid = make_shared<Grid>(10, vec4(1, 1, 1, 1), true);

    window->setMouseGrab(true);
}

Game::~Game() {}

void Game::onKeyReleased(const sf::Event::KeyEvent & event) {
    GameBase::onKeyReleased(event);
    switch (event.code) {
        case sf::Keyboard::G:
            drawGrid = !drawGrid;
            break;
        default:
            GameBase::onKeyReleased(event);
            break;
    }
}

void Game::onUpdate(const sf::Time & delta) {
    float s = delta.asSeconds();
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
    glLineWidth(1.0);
}

void Game::onDraw() const {
    setupGl();

    RenderState state(camera);
    state.setGridEnable(drawGrid);
    scene.draw(state);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glLineWidth(4.0);

    glpp::BufferArray::unbind();
}

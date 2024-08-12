#include "Game.hpp"

#include <stdexcept>

Game::Game(Window::Ptr & window)
    : GameBase(window),
      res("../../../examples/res"),
      shader(res.getMVPShader("shader/default.vert", "shader/default.frag")),
      grid(10, {1, 1, 1, 1}, true),
      tPillar(0) {

    camera.setPosition({5, 2, 5});
    camera.setRotation({0.2, -0.75, 0});

    shared_ptr<Scene> modelScene;

    modelScene = scene.addChild();
    modelScene->models = res.loadModel("model/plane.obj");

    pillar = scene.addChild();
    pillar->models = res.loadModel("model/pillar.obj");

    for (auto & s : scene.children) {
        for (auto & m : s->models) m->material->shader = shader;
    }

    // Load models / textures / scenes
    // No fancy render api, just each model can be drawn
    // Maybe add something like pyglet Batch to group rendering

    window->setMouseGrab(false);
}

Game::~Game() {}

void Game::onMouseDown(const sf::Event::MouseButtonEvent & event) {
    switch (event.button) {
        case sf::Mouse::Button::Right:
            window->setMouseGrab(true);
            break;
    }
}

void Game::onMouseUp(const sf::Event::MouseButtonEvent & event) {
    switch (event.button) {
        case sf::Mouse::Button::Right:
            window->setMouseGrab(false);
            break;
    }
}

void Game::onUpdate(const sf::Time & delta) {
    float s = delta.asSeconds();
    tPillar += s;

    pillar->models[1]->transform.rotateEuler({0, s * 0.5, 0});
    pillar->models[1]->transform.move({0, sin(tPillar) * 0.005, 0});
    pillar->models[2]->transform.rotateEuler({0, s * -0.25, 0});
    pillar->models[2]->transform.move({0, sin(tPillar) * 0.01, 0});
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

    RenderState state(camera);
    scene.draw(state);

    grid.draw(state.getMVP());

    glpp::BufferArray::unbind();
}

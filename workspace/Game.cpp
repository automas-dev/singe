#include "Game.hpp"

#include <stdexcept>

using std::make_shared;

Game::Game(Window & window)
    : GameBase(window),
      res("../../examples/res"),
      shader(res.getShader("default")),
      grid(10, {1, 1, 1, 1}, true) {

    sf::Text loadingText;
    loadingText.setFont(uiFont);
    loadingText.setString("Loading");
    loadingText.setCharacterSize(32);
    loadingText.setFillColor(sf::Color(200, 200, 200));
    loadingText.setOrigin(loadingText.getLocalBounds().left,
                          loadingText.getLocalBounds().top);
    loadingText.setPosition(100, 100);
    window.window.draw(loadingText);
    window.display();

    camera.setPosition({5, 2, 5});
    camera.setRotation({0.2, -0.75, 0});

    // TODO: load here

    shared_ptr<Scene> modelScene;

    modelScene = scene.addChild();
    modelScene->models = res.loadModel("sphere.obj");
    modelScene->transform.move({0, 1, 0});

    modelScene = scene.addChild();
    modelScene->models = res.loadModel("plane.obj");
    modelScene->transform.move({0, 0, 0});

    modelScene = scene.addChild();
    modelScene->models = res.loadModel("cube.obj");
    modelScene->transform.move({0, 1, 3});

    modelScene = scene.addChild();
    modelScene->models = res.loadModel("fountain.obj");
    modelScene->transform.move({2, 0, -3});

    modelScene = scene.addChild();
    modelScene->models = res.loadModel("Human.obj");
    modelScene->transform.move({-2, 0, -3});

    for (auto & s : scene.children) {
        for (auto & m : s->models) m->material->shader = shader;
    }

    // modelScene->material->shader = shader;

    // Load models / textures / scenes
    // No fancy render api, just each model can be drawn
    // Maybe add something like pyglet Batch to group rendering

    window.setMouseGrab(true);
}

Game::~Game() {}

void Game::onUpdate(const sf::Time & delta) {
    float s = delta.asSeconds();
    // TODO: update here
    scene.transform.rotateEuler({0, s * 0.5, 0});
    for (auto & child : scene.children)
        child->transform.rotateEuler({0, -s, 0});
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

    glm::mat4 vp = camera.projMatrix() * camera.viewMatrix();
    RenderState state(vp);
    scene.draw(state);

    grid.draw(vp);

    glpp::BufferArray::unbind();
}

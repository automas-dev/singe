#include "Game.hpp"

#include <stdexcept>

Game::Game(Window & window)
    : GameBase(window),
      res("../../examples/res"),
      shader(res.getShader("default")),
      gridShader(res.getShader("grid")),
      grid(10, {1, 1, 1, 1}, true) {

    camera.setPosition({5, 2, 5});
    camera.setRotation({0.2, -0.75, 0});

    // TODO: load here

    shared_ptr<Mesh> model;

    model = scene.models.emplace_back(res.loadModel("sphere.obj"));
    model->transform.move({0, 1, 0});
    model->material->shader = shader;

    model = scene.models.emplace_back(res.loadModel("plane.obj"));
    model->transform.move({0, 0, 0});
    model->material->shader = shader;

    model = scene.models.emplace_back(res.loadModel("cube.obj"));
    model->transform.move({0, 1, 3});
    model->material->shader = shader;

    model = scene.models.emplace_back(res.loadModel("fountain.obj"));
    model->transform.move({2, 0, -3});
    model->material->shader = shader;

    model = scene.models.emplace_back(res.loadModel("Human.obj"));
    model->transform.move({-2, 0, -3});
    model->material->shader = shader;

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
    for (auto & model : scene.models)
        model->transform.rotateEuler({0, -s, 0});
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
    // defaultShader.bind();
    // shader->mvp().setMat4(vp);

    RenderState state(vp);
    scene.draw(state);

    // gridShader.bind();
    // Uniform gridMvp = gridShader.uniform("mvp");
    // gridMvp.setMat4(vp);
    // grid.draw();

    glpp::BufferArray::unbind();
}

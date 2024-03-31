#include "Game.hpp"

#include <stdexcept>

using std::make_shared;
using glpp::extra::Grid;

static glm::vec2 reverseProject(glm::vec3 & point, glm::mat4 & mvp) {
    auto projPoint = mvp * vec4(point, 1.0);
    projPoint /= projPoint.w;
    return vec3(projPoint.x, projPoint.y, 0.0);
}

Game::Game(Window::Ptr & window)
    : GameBase(window), res("../../../examples/res") {

    shader = res.getMVPShader("shader/default.vert", "shader/default.frag");

    circle = make_shared<Diamond>(vec2(0.02), vec4(1.0, 0.0, 0.0, 1.0));
    circle->setPos({0.5, 0.9});

    camera.setPosition({5, 2, 5});
    camera.setRotation({0.2, -0.75, 0});

    scene.grid = make_shared<Grid>(10, vec4(1, 1, 1, 1), true);

    line = make_shared<glpp::extra::Line>(vec3(0, 0, 0), vec3(1, 2, 3),
                                          vec4(1.0, 0.0, 1.0, 1.0));

    scene.models = res.loadModel("model/angle_cube.obj");
    scene.models[0]->material->shader = shader;

    window->setMouseGrab(true);
}

Game::~Game() {}

void Game::onUpdate(const sf::Time & delta) {
    float s = delta.asSeconds();
    scene.models[0]->transform.rotateEuler({0, s * 0.5, 0});

    glm::mat4 mvp = camera.projMatrix() * camera.viewMatrix()
                    * scene.transform.toMatrix()
                    * scene.models[0]->transform.toMatrix();
    vec2 point = reverseProject(scene.models[0]->points[0].pos, mvp);
    circle->setPos(point);
    float r = (float)camera.getScreenSize().x / (float)camera.getScreenSize().y;
    circle->setSize({circle->getSize().x, circle->getSize().x * r});
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

    line->draw(mvp);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    circle->draw();

    glpp::BufferArray::unbind();
}

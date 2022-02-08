#include "Game.hpp"

#include <stdexcept>

void GLAPIENTRY MessageCallback(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar * message,
                                const void * userParam) {
    Logging::Game->error(
        "GL CALLBACK: {} type = 0x{:x}, severity = 0x{:x}, message = {}",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity,
        message);
}

Game::Game(Window & window) : GameBase(window) {

#ifdef DEBUG
    // During init, enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
#endif

    camera->move({5, 2, 5});
    camera->setFov(70);

    // TODO: load here

    window.setMouseGrab(true);
}

Game::~Game() {}

void Game::onUpdate(const sf::Time & delta) {
    // TODO: update here
}

void Game::onDraw() const {
    glClearColor(0.25, 0.25, 0.25, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 vp = camera->projMatrix() * camera->toMatrix();
    defaultShader.bind();
    defaultShader.setMat4("mvp", vp);

    // TODO: draw here
}

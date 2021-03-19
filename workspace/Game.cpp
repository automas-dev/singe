#include "Game.hpp"
#include <exception>
#include <spdlog/spdlog.h>


static void getGlError() {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        SPDLOG_ERROR("glGetError() returned {}", err);
    }
}

Game::Game(const sf::String & resPath) : GameBase(), resManager(resPath) { }

Game::~Game() { }

void GLAPIENTRY MessageCallback( GLenum source,
                                 GLenum type,
                                 GLuint id,
                                 GLenum severity,
                                 GLsizei length,
                                 const GLchar *message,
                                 const void *userParam ) {
    SPDLOG_ERROR("GL CALLBACK: {} type = 0x{:x}, severity = 0x{:x}, message = {}",
                 ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
                 type, severity, message );
}

bool Game::onCreate() {
    // defautFont loaded from memory by GameBase
    fps = std::make_shared<FPSDisplay>();
    fps->setFont(uiFont);
    fps->setRate(0.1f);

    // During init, enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    // Add menu buttons
    menu->addMenuItem("Resume", [&]() {
        menu->hide();
        SetMouseGrab(true);
    });
    menu->addMenuItem("Exit", [&]() {
        window->close();
    });

    // Initialize camera to look at origin
    camera->move({-3, 2, -1});
    camera->rotate({0, 110});
    camera->setFov(80);

    shader = resManager.loadShader("shader/shader.vert", "shader/shader.frag");
    if (!shader)
        return false;

    devTexture = resManager.loadTexture("devTexture", "img/uv.png");
    if (!devTexture)
        return false;
    devTexture->setFilter(GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);

    glm::vec4 offset_x(0.1, 0.0, 0.1, 0.0);
    glm::vec4 offset_y(0.0, 0.1, 0.0, 0.1);
    glm::vec4 offset = offset_x + offset_y;
    glm::vec4 origin = {0.1, 0.1, 0.2, 0.2};
    UV north (origin);
    UV east (origin + offset_x);
    UV south (origin + offset_x + offset_x);
    UV west (origin - offset_x);
    UV top (origin - offset_y);
    UV bottom (origin + offset_y);
    Cube c({0, 0, 0}, north, east, south, west, bottom, top);

    Chunk ck;

    model = std::make_shared<Model>();
    bool res = model->loadFromPoints(ck.toPoints());
    if (!res)
        return false;

    SetMouseGrab(true);
    getGlError();
    return true;
}

void Game::onDestroy() { }

void Game::onKeyPressed(const sf::Event::KeyEvent & e) {
    GameBase::onKeyPressed(e);
}

void Game::onKeyReleased(const sf::Event::KeyEvent & e) {
    GameBase::onKeyReleased(e);
}

void Game::onMouseMove(const sf::Event::MouseMoveEvent & e) {
    GameBase::onMouseMove(e);
}

void Game::onMouseDown(const sf::Event::MouseButtonEvent & e) {
    GameBase::onMouseDown(e);
}

void Game::onMouseUp(const sf::Event::MouseButtonEvent & e) {
    GameBase::onMouseUp(e);
}

void Game::onResized(const sf::Event::SizeEvent & e) {
    GameBase::onResized(e);
}

void Game::onUpdate(const sf::Time & delta) {
    float deltaS = delta.asSeconds();
    fps->update(delta);
}

void Game::onDraw() const {

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glPointSize(10);

    glm::mat4 vp = camera->projMatrix() * camera->viewMatrix();

    shader->bind();
    devTexture->bind();
    shader->setMat4("mvp", vp);
    shader->setMat4("model", model->modelMatrix());
    {
        model->draw();

        glBegin(GL_POINTS);
        {
            glVertex3d(0, 0, 0);
            glVertex3d(1, 0, 0);
            glVertex3d(0, 1, 0);
            glVertex3d(0, 0, 1);
            glVertex3d(-0.5, 0, 0);
            //glVertex3d(0, 0, -0.5);
        }
        glEnd();
    }
    shader->unbind();
    devTexture->unbind();


    window->pushGLStates();
    window->draw(*fps);
    window->popGLStates();
}


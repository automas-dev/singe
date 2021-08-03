#include "Game.hpp"

#include <spdlog/spdlog.h>

#include <exception>
#include <glm/gtc/noise.hpp>


static void getGlError() {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        SPDLOG_ERROR("glGetError() returned {}", err);
    }
}

Game::Game(const sf::String & resPath) : GameBase(), resManager(resPath) {}

Game::~Game() {}

void GLAPIENTRY MessageCallback(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar * message,
                                const void * userParam) {
    SPDLOG_ERROR("GL CALLBACK: {} type = 0x{:x}, severity = 0x{:x}, message = {}",
                 (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type,
                 severity, message);
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
    camera->rotate({0, 110, 0});
    camera->setFov(70);

    devTexture = resManager.loadTexture("img/uv.png");
    if (!devTexture)
        return false;
    devTexture->setFilter(GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);

    std::vector<Vertex> floorPoints {
        Vertex({-50, 50, -50}, {0, 1, 0}, {0, 1}),
        Vertex({50, 50, 50}, {0, 1, 0}, {1, 0}),
        Vertex({50, 50, -50}, {0, 1, 0}, {1, 1}),

        Vertex({-50, 50, -50}, {0, 1, 0}, {0, 1}),
        Vertex({-50, 50, 50}, {0, 1, 0}, {0, 0}),
        Vertex({50, 50, 50}, {0, 1, 0}, {1, 0}),
    };

    floorModel = std::make_shared<Model>();
    floorModel->loadFromPoints(floorPoints);
    floorModel->move({0, -56, 0});

    objectModel = resManager.loadModel("model/sphere.obj");
    if (!objectModel)
        return false;

    scene = Scene::loadFrom(resManager.resourceAt("model/cube_plane.obj"));
    if (!scene)
        return false;
    // for (auto & mesh : scene->models) {
    //     Logging::Core->debug("Scene has mesh with {} points",
    //     mesh->mesh->points.size()); mesh->textures.push_back(devTexture);
    // }

    SetMouseGrab(true);
    getGlError();
    return true;
}

void Game::onDestroy() {}

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

    glm::mat4 vp = camera->projMatrix() * camera->toMatrix();

    defaultShader->bind();
    devTexture->bind();
    defaultShader->setMat4("mvp", vp);
    {
        defaultShader->setMat4("model", floorModel->toMatrix());
        // floorModel->draw();

        defaultShader->setMat4("model", objectModel->toMatrix());
        objectModel->draw();

        scene->draw(defaultShader);
    }
    defaultShader->unbind();
    devTexture->unbind();


    window->pushGLStates();
    window->draw(*fps);
    window->popGLStates();
}

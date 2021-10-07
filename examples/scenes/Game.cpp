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

Game::Game(const sf::String & resPath)
    : GameBase(), resManager(resPath), taskQueue(DispatchQueue::Concurrent) {}

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

void Game::onCreate() {
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
    camera->move({5, 2, 5});
    camera->rotateEuler({0, -1, 0});
    camera->setFov(70);

    auto devTexture =
        resManager.loadTexture("img/uv.png", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
    if (!devTexture)
        throw std::runtime_error("Failed to load img/uv.png");

    auto objectScene = resManager.loadScene("model/sphere.obj");
    if (!objectScene)
        throw std::runtime_error("Failed to load model/sphere.obj");
    objectScene->move({0, 2, 0});

    scene = resManager.loadScene("model/cube_plane.obj");
    if (!scene)
        throw std::runtime_error("Failed to load model/cube_plane.obj");

    scene->children.push_back(objectScene);

    otherScene = std::make_shared<Scene>("other");
    scene->children.push_back(otherScene);

    taskQueue.add([&] {
        auto fountainScene = resManager.loadScene("model/fountain.obj");
        if (!fountainScene)
            return;
        fountainScene->move({0, 0, 3});
        localTaskQueue.add([&, fountainScene] {
            fountainScene->send();
            otherScene->children.push_back(fountainScene);
        });
    });

    auto humanScene = resManager.loadScene("model/Human.obj");
    if (!humanScene)
        throw std::runtime_error("Failed to load model/Human.obj");
    humanScene->move({3, 0, 0});
    otherScene->children.push_back(humanScene);

    scene->send();

    SetMouseGrab(true);
    getGlError();
}

void Game::onDestroy() {
    taskQueue.stop();
}

void Game::onUpdate(const sf::Time & delta) {
    float deltaS = delta.asSeconds();
    fps->update(delta);

    // scene->rotate(glm::quat(glm::vec3(0, 0.5 * delta.asSeconds(), 0)));
    scene->children[0]->rotateEuler({delta.asSeconds(), delta.asSeconds() * 0.2, 0});
    otherScene->rotateEuler({0, delta.asSeconds() * 0.1, 0});

    localTaskQueue.processAll();
}

void Game::onDraw() const {

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glm::mat4 vp = camera->projMatrix() * camera->toMatrix();

    defaultShader->setMat4("mvp", vp);
    scene->draw(defaultShader);

    window->pushGLStates();
    window->draw(*fps);
    window->popGLStates();
}

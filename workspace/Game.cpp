#include "Game.hpp"

#include <stdexcept>

Game::Game(const sf::String & resPath) : GameBase(), resManager(resPath) {}

Game::~Game() {}

void GLAPIENTRY MessageCallback(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar * message,
                                const void * userParam) {
    Logging::Game->error("GL CALLBACK: {} type = 0x{:x}, severity = 0x{:x}, message = {}",
                 (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type,
                 severity, message);
}

void Game::onCreate() {
    // defautFont loaded from memory by GameBase
    fps = std::make_shared<FPSDisplay>();
    fps->setFont(uiFont);
    fps->setRate(0.1f);

#ifdef DEBUG
    // During init, enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
#endif

    // Add menu buttons
    menu->addMenuItem("Resume", [&]() {
        menu->hide();
        SetMouseGrab(true);
    });
    menu->addMenuItem("Exit", [&]() {
        window->close();
    });

    camera->move({5, 2, 5});
    camera->setFov(70);

    scene = std::make_shared<Scene>("Root");

    auto floorScene = resManager.loadScene("model/cube_plane.obj");
    if (!floorScene)
        throw std::runtime_error("Failed to load model/cube_plane.obj");
    scene->children.push_back(floorScene);
    scene->send();

    auto sphereScene = resManager.loadScene("model/sphere.obj");
    if (!sphereScene)
        throw std::runtime_error("Failed to load model/sphere.obj");
    sphereScene->scale({0.1, 0.1, 0.1});
    sphereScene->move({1, 2, 3});

    auto cubeScene = resManager.loadScene("model/cube.obj");
    if (!cubeScene)
        throw std::runtime_error("Failed to load model/cube.obj");
    cubeScene->move({3, 0, 0});

    scene->children.push_back(sphereScene);
    scene->children.push_back(cubeScene);
    scene->send();

    SetMouseGrab(true);
}

void Game::onDestroy() {}

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 vp = camera->projMatrix() * camera->toMatrix();

    defaultShader->bind();
    defaultShader->setMat4("mvp", vp);
    scene->draw(defaultShader);

    window->pushGLStates();
    window->draw(*fps);
    window->popGLStates();
}

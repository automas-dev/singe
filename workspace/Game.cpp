#include "Game.hpp"

#include <stdexcept>

// #include "SceneParse.hpp"

Game::Game(Window & window, const sf::String & resPath)
    // : GameBase(window), resManager(resPath) {
    : GameBase(window) {

    // defautFont loaded from memory by GameBase
    // fps = std::make_shared<FPSDisplay>();
    // fps->setFont(uiFont);
    // fps->setRate(0.1f);

#ifdef DEBUG
    // During init, enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
#endif

    // Add menu buttons
    // menu->addMenuItem("Resume", [&]() {
    //     menu->hide();
    //     window.setMouseGrab(true);
    // });
    // menu->addMenuItem("Exit", [&]() {
    //     window.close();
    // });

    // SceneStruct sceneStruct = parseScene("scene.xml", resManager);
    // grid = sceneStruct.grid;

    camera->move({5, 2, 5});
    camera->setFov(70);

    // scene = std::make_shared<Scene>("Root");

    // auto floorScene = resManager.loadScene("model/cube_plane.obj");
    // if (!floorScene)
    //     throw std::runtime_error("Failed to load model/cube_plane.obj");
    // scene->children.push_back(floorScene);
    // scene->move({0, -1, 0});

    // grid = std::make_shared<Grid>(50, 1.0, glm::vec3(0.47, 0.6, 0.81));

    window.setMouseGrab(true);
}

Game::~Game() {}

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

void Game::onUpdate(const sf::Time & delta) {
    float deltaS = delta.asSeconds();
    // fps->update(delta);
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
    // RenderState state(camera, defaultShader, vp);
    // if (grid)
    //     grid->draw(state);

    defaultShader.bind();
    defaultShader.setMat4("mvp", vp);
    // scene->draw(state);

    // window->pushGLStates();
    // window->draw(*fps);
    // window->popGLStates();
}

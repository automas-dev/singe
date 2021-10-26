#include "Game.hpp"

#include <stdexcept>

static const std ::string gridFragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 camera;
in vec3 FragPos;
in vec3 FragNorm;
in vec2 FragTex;
void main() {
    float h = abs(camera.y);
    float d = distance(FragPos, vec3(camera.x, 0.0, camera.z));
    float start = -h/tan(radians(-20));
    float end = -h/tan(radians(-5));
    float a = (d-end) / (start-end);
    a = clamp(a, 0, 1);
    if (FragPos.x == 0) {
        FragColor = vec4(1.0, 0.0, 0.0, a);
    }
    else if (FragPos.z == 0) {
        FragColor = vec4(0.0, 0.0, 1.0, a);
    }
    else {
        FragColor = vec4(0.4, 0.4, 0.4, a);
    }
})";

Game::Game(const sf::String & resPath) : GameBase(), resManager(resPath) {}

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

    gridShader = Shader::fromFragmentSource(gridFragmentShaderSource);
    if (!gridShader)
        throw std::runtime_error("Failed to load internal grid shader");

    scene = std::make_shared<Scene>("Root");

    auto floorScene = resManager.loadScene("model/cube_plane.obj");
    if (!floorScene)
        throw std::runtime_error("Failed to load model/cube_plane.obj");
    scene->children.push_back(floorScene);
    scene->move({0, -1, 0});
    scene->send();

    grid = std::make_shared<Grid>(100);

    SetMouseGrab(true);
}

void Game::onDestroy() {}

void Game::onUpdate(const sf::Time & delta) {
    float deltaS = delta.asSeconds();
    fps->update(delta);
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

    gridShader->bind();
    gridShader->setMat4("mvp", vp);
    gridShader->setMat4("model", glm::mat4(1));
    gridShader->setVec3("camera", camera->getPosition());
    grid->draw();

    defaultShader->bind();
    defaultShader->setMat4("mvp", vp);
    scene->draw(defaultShader);

    window->pushGLStates();
    window->draw(*fps);
    window->popGLStates();
}

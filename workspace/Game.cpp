#include "Game.hpp"

#include <spdlog/spdlog.h>

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

bool Game::onCreate() {
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
    camera->rotateEuler({0, -1, 0});
    camera->setFov(70);

    gBuff = std::make_shared<FrameBuffer>(
        window->getSize(),
        std::vector<FrameBufferAttachment> {
            FrameBufferAttachment(GL_COLOR_ATTACHMENT0, GL_RGB16F, GL_RGB, GL_FLOAT),
            FrameBufferAttachment(GL_COLOR_ATTACHMENT1, GL_RGB16F, GL_RGB, GL_FLOAT),
            FrameBufferAttachment(GL_COLOR_ATTACHMENT2, GL_RGB, GL_RGB, GL_FLOAT),
            FrameBufferAttachment(GL_COLOR_ATTACHMENT3, GL_R16F, GL_RED, GL_FLOAT),
        },
        true, 0);

    gShader = resManager.loadShader("shader/geom.vert", "shader/geom copy.frag");
    if (!gShader)
        return false;

    scene = std::make_shared<Scene>("Root");

    auto floorScene = resManager.loadScene("model/cube_plane.obj");
    if (!floorScene)
        return false;
    scene->children.push_back(floorScene);
    scene->send();

    taskQueue.add([&] {
        auto fountainScene = resManager.loadScene("model/fountain.obj");
        if (!fountainScene)
            return;
        fountainScene->move({0, 0, 3});

        auto cubeScene = resManager.loadScene("model/cube.obj");
        if (!cubeScene)
            return;

        localTaskQueue.add([=] {
            scene->children.push_back(fountainScene);
            scene->children.push_back(cubeScene);
            scene->send();
        });
    });

    SetMouseGrab(true);
    return true;
}

void Game::onDestroy() {
    taskQueue.stop();
}

void Game::onKeyPressed(const sf::Event::KeyEvent & e) {
    GameBase::onKeyPressed(e);
    switch (e.code) {
        case sf::Keyboard::Num1:
            drawMode = 0;
            break;
        case sf::Keyboard::Num2:
            drawMode = 1;
            break;
        case sf::Keyboard::Num3:
            drawMode = 2;
            break;
        case sf::Keyboard::Num4:
            drawMode = 3;
            break;
        case sf::Keyboard::Num0:
            drawMode = 4;
            break;
        default:
            break;
    }
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
    localTaskQueue.processAll();
}

void Game::onDraw() const {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glm::mat4 vp = camera->projMatrix() * camera->toMatrix();

    gBuff->bind();
    gShader->bind();
    gShader->setMat4("mvp", vp);
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene->draw(gShader);
    }
    gBuff->unbind();

    defaultShader->bind();
    if (drawMode == 4) {
        defaultShader->setMat4("mvp", vp);
        scene->draw(defaultShader);
    }
    else {
        defaultShader->setMat4("mvp", glm::mat4(1));
        defaultShader->setMat4("model", glm::mat4(1));
        auto & texs = gBuff->getTextures();
        texs[drawMode]->bind();
        draw_quad({-1, -1}, {2, 2});
    }

    window->pushGLStates();
    window->draw(*fps);
    window->popGLStates();
}

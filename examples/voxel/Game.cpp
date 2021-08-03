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
    camera->move({-3, 22, -1});
    camera->rotateEuler({0, 1, 0});
    camera->setFov(70);

    devTexture = resManager.loadTexture("img/uv.png");
    if (!devTexture)
        return false;
    devTexture->setFilter(GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);

    chunks = std::make_shared<ChunkManager>();
    for (int x = 0; x < 5; x++)
        for (int z = 0; z < 5; z++)
            chunks->loader.load(x * SubChunk::N, z * SubChunk::N);
    model = std::make_shared<VBO>(VBO::Triangles, VBO::Dynamic);

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

    if (chunks->loader.hasNext()) {
        while (chunks->loader.hasNext()) {
            auto chunk = chunks->loader.next();
            SPDLOG_DEBUG("Got a chunk {}, {} in update", chunk->pos.x,
                         chunk->pos.z);
            auto key = std::make_pair<int, int>(chunk->pos.x, chunk->pos.z);
            chunks->chunks[key] = chunk;
        }
        model->loadFromPoints(chunks->toPoints());
    }
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
    // defaultShader->setMat4("model", model->modelMatrix());
    defaultShader->setMat4("model", glm::mat4(1));
    {
        model->draw();

        glBegin(GL_POINTS);
        {
            glVertex3d(0, 0, 0);
            glVertex3d(1, 0, 0);
            glVertex3d(0, 1, 0);
            glVertex3d(0, 0, 1);
            glVertex3d(-0.5, 0, 0);
            // glVertex3d(0, 0, -0.5);
        }
        glEnd();
    }
    defaultShader->unbind();
    devTexture->unbind();


    window->pushGLStates();
    window->draw(*fps);
    window->popGLStates();
}

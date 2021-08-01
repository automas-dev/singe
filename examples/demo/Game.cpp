#include "Game.hpp"

#include <spdlog/spdlog.h>

static std::vector<glm::vec3> genGridVerts(int steps = 10) {
    std::vector<glm::vec3> verts;
    for (int x = -steps; x <= steps; x++) {
        if (x == 0) {
            verts.push_back({0, 0, -steps});
            verts.push_back({0, 0, 0});
            verts.push_back({0, 0, 0});
            verts.push_back({0, 0, steps});
        }
        else {
            verts.push_back({x, 0, -steps});
            verts.push_back({x, 0, steps});
        }
    }
    for (int z = -steps; z <= steps; z++) {
        if (z == 0) {
            verts.push_back({-steps, 0, 0});
            verts.push_back({0, 0, 0});
            verts.push_back({0, 0, 0});
            verts.push_back({steps, 0, 0});
        }
        else {
            verts.push_back({-steps, 0, z});
            verts.push_back({steps, 0, z});
        }
    }
    verts.push_back({0, 0, 0});
    verts.push_back({0, steps, 0});
    return verts;
}

static std::vector<glm::vec3> genGridCols(int steps = 10) {
    std::vector<glm::vec3> cols;
    glColor3f(1.0, 1.0, 1.0);
    for (int x = -steps; x <= steps; x++) {
        if (x == 0) {
            cols.push_back({1.0, 1.0, 1.0});
            cols.push_back({1.0, 1.0, 1.0});
            cols.push_back({0.0, 0.0, 1.0});
            cols.push_back({0.0, 0.0, 1.0});
        }
        else {
            cols.push_back({1.0, 1.0, 1.0});
            cols.push_back({1.0, 1.0, 1.0});
        }
    }
    for (int z = -steps; z <= steps; z++) {
        if (z == 0) {
            cols.push_back({1.0, 1.0, 1.0});
            cols.push_back({1.0, 1.0, 1.0});
            cols.push_back({1.0, 0.0, 0.0});
            cols.push_back({1.0, 0.0, 0.0});
        }
        else {
            cols.push_back({1.0, 1.0, 1.0});
            cols.push_back({1.0, 1.0, 1.0});
        }
    }
    cols.push_back({0.0, 1.0, 0.0});
    cols.push_back({0.0, 1.0, 0.0});
    return cols;
}

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
    if (!font.loadFromFile(resManager.resourceAt("Questrial_Regular.ttf"))) {
        SPDLOG_ERROR("failed to load Questrial_Regular.ttf");
        return false;
    }

    fps = std::make_shared<FPSDisplay>();
    fps->setFont(font);
    fps->setRate(0.1f);

    // During init, enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    gridVerts = genGridVerts(10);
    gridCols = genGridCols(10);

    menu->addMenuItem("New", [&]() {
        menu->hide();
        SetMouseGrab(true);
    });
    menu->addMenuItem("Load", []() {});
    menu->addMenuItem("Options", []() {});
    menu->addMenuItem("Exit", [&]() {
        window->close();
    });

    camera->move({3, 2, 1});
    camera->rotate({30, -70, 0});
    camera->setFov(80);

    defaultShader =
        resManager.loadShader("shader/default.vs", "shader/default.fs");
    if (!defaultShader) {
        throw std::runtime_error("Failed to load default shader");
    }

    geometryShader = std::make_shared<MaterialShader>();
    if (!geometryShader->loadFromPath(resManager.resourceAt("shader/geom.vert"),
                                      resManager.resourceAt("shader/geom.frag"))) {
        throw std::runtime_error("Failed to load texture shader");
    }

    debugShader = resManager.loadShader("shader/debug.vert", "shader/debug.frag");
    if (!debugShader) {
        throw std::runtime_error("Failed to load debug shader");
    }

    lightingShader = std::make_shared<MaterialShader>();
    if (!lightingShader->loadFromPath(
            resManager.resourceAt("shader/lighting.vert"),
            resManager.resourceAt("shader/lighting.frag"))) {
        throw std::runtime_error("Failed to load lighting shader");
    }

    monoShader = std::make_shared<MaterialShader>();
    if (!monoShader->loadFromPath(resManager.resourceAt("shader/mono.vs"),
                                  resManager.resourceAt("shader/mono.fs"))) {
        throw std::runtime_error("Failed to load mono shader");
    }

    cubeModel = resManager.loadModel("model/cube_plane.obj");
    if (!cubeModel) {
        throw std::runtime_error("Failed to load cube model");
    }

    sphereModel = resManager.loadModel("model/sphere.obj");
    if (!sphereModel) {
        throw std::runtime_error("Failed to load sphere model");
    }
    sphereModel->move({1, 2, 3});
    sphereModel->scale({0.1, 0.1, 0.1});

    hallModel = resManager.loadModel("model/hall.obj");
    if (!hallModel) {
        throw std::runtime_error("Failed to load hall model");
    }
    hallModel->move({0, 0, -4});

    texture = resManager.loadTexture("img/dev_texture_gray.png");
    if (!texture) {
        throw std::runtime_error("Failed to load dev texture");
    }

    light0 = {
        .index = 0,
        .ambient = {0.1, 0.1, 0.1},
        .diffuse = {0.8, 0.8, 0.8},
        .specular = {1, 1, 1},
        .position = {0, 0, 0},
        .direction = {-1, -2, -3},
        .type = Light::POINT,
        .constant = 1.0,
        .linear = 0.09,
        .quadratic = 0.032,
    };

    light1 = {.index = 1,
              .ambient = {0.1, 0.1, 0.1},
              .diffuse = {0.8, 0.8, 0.8},
              .specular = {1, 1, 1},
              .position = {0, 0, 0},
              .direction = {-1, -2, -3},
              .type = Light::SPOT,
              .constant = 1.0,
              .linear = 0.09,
              .quadratic = 0.032,
              .cutOff = glm::cos(glm::radians(12.5f)),
              .outerCutOff = glm::cos(glm::radians(15.0f))};

    light2 = {.index = 2,
              .ambient = {0.1, 0.1, 0.1},
              .diffuse = {2.0, 2.0, 2.0},
              .specular = {3.0, 3.0, 3.0},
              .position = {0, 1, -9},
              .direction = {0, 0, -1},
              .type = Light::SPOT,
              .constant = 1.0,
              .linear = 0.09,
              .quadratic = 0.032,
              .cutOff = glm::cos(glm::radians(12.5f)),
              .outerCutOff = glm::cos(glm::radians(15.0f))};

    fbuff = std::make_shared<FrameBuffer>(
        window->getSize(),
        std::vector<FrameBufferAttachment> {
            FrameBufferAttachment(GL_COLOR_ATTACHMENT0),
        },
        true, 8);

    gbuffMulti = std::make_shared<FrameBuffer>(
        window->getSize(),
        std::vector<FrameBufferAttachment> {
            FrameBufferAttachment(GL_COLOR_ATTACHMENT0, GL_RGBA16F, GL_RGBA, GL_FLOAT),
            FrameBufferAttachment(GL_COLOR_ATTACHMENT1, GL_RGBA16F, GL_RGBA, GL_FLOAT),
            FrameBufferAttachment(GL_COLOR_ATTACHMENT2, GL_RGBA, GL_RGBA, GL_FLOAT),
        },
        true, 8);

    SetMouseGrab(true);

    getGlError();
    return true;
}

void Game::onDestroy() {}

void Game::onKeyPressed(const sf::Event::KeyEvent & e) {
    switch (e.code) {
        case sf::Keyboard::Num1:
            doDrawGrid = !doDrawGrid;
            break;
        case sf::Keyboard::Num2:
            drawGridOver = !drawGridOver;
            break;
        case sf::Keyboard::Num3:
            doDrawDebug = !doDrawDebug;
            break;
        case sf::Keyboard::Num4:
            doBlitFbuff = !doBlitFbuff;
            break;
        default:
            GameBase::onKeyPressed(e);
            break;
    }
    keymap.onKeyPressed(e);
}

void Game::onKeyReleased(const sf::Event::KeyEvent & e) {
    GameBase::onKeyReleased(e);
    keymap.onKeyReleased(e);
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
    gbuffMulti->setSize({e.width, e.height});
    fbuff->setSize({e.width, e.height});
}

void Game::onUpdate(const sf::Time & delta) {
    float deltaS = delta.asSeconds();
    fps->update(delta);
    time += deltaS;
    timeSinceLastTick += deltaS;

    float spt = 1 / tps;
    if (timeSinceLastTick >= spt) {
        timeSinceLastTick -= spt;
        tickTimer.restart();
        tick();
        auto tickTime = tickTimer.getElapsedTime();
        SPDLOG_TRACE("tick took {} mss", tickTime.asMilliseconds());
    }
}

void Game::tick() {
    sphereModel->setPosition({glm::cos(time) * 3, 2, glm::sin(time) * 3});
    // cubeModel->setRotation({time, time / 3, time / 7});

    light0.position = sphereModel->getPosition();
    light1.position = sphereModel->getPosition();
}

void Game::onDraw() const {

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glm::mat4 vp = camera->projMatrix() * camera->toMatrix();

    geometryShader->bind();
    if (true) {
        gbuffMulti->bind();
        texture->bind();

        geometryShader->setFloat("specular", 0.8);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        geometryShader->setMat4("mvp", vp);

        drawPass(geometryShader);

        texture->unbind();
        gbuffMulti->unbind();
    }

    std::shared_ptr<FrameBuffer> gbuff(nullptr);
    if (gbuffMulti->isMultisampled())
        gbuff = gbuffMulti->getResovled();
    else
        gbuff = gbuffMulti;

    fbuff->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lightingShader->bind();
    {
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);

        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        gbuff->getTextures()[0]->bind();
        glActiveTexture(GL_TEXTURE1);
        gbuff->getTextures()[1]->bind();
        glActiveTexture(GL_TEXTURE2);
        gbuff->getTextures()[2]->bind();

        lightingShader->setInt("gPosition", 0);
        lightingShader->setInt("gNormal", 1);
        lightingShader->setInt("gAlbedo", 2);

        lightingShader->setMat4("vp", vp);
        lightingShader->setVec3("viewPos", camera->getPosition());
        lightingShader->setUInt("nLights", 3);

        light0.uniform(lightingShader);
        light1.uniform(lightingShader);
        light2.uniform(lightingShader);

        draw_quad({-1, -1}, {2, 2});

        glActiveTexture(GL_TEXTURE0);
        glDisable(GL_TEXTURE_2D);
    }

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    std::shared_ptr<FrameBuffer> fbuffTmp(nullptr);
    if (fbuff->isMultisampled())
        fbuffTmp = fbuff->getResovled();
    else
        fbuffTmp = fbuff;

    debugShader->bind();
    if (doBlitFbuff) {
        fbuff->blit(0, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        fbuff->unbind();
    }
    else {
        fbuff->unbind();
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        fbuffTmp->getTextures()[0]->bind();

        debugShader->setInt("gPosition", 0);
        debugShader->setInt("show", 0);
        debugShader->setInt("tonemap", 0);

        draw_quad({-1, -1}, {2, 2});

        fbuffTmp->getTextures()[0]->unbind();
        glDisable(GL_TEXTURE_2D);
    }

    debugShader->bind();
    if (doDrawDebug) {
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        gbuff->getTextures()[0]->bind();
        glActiveTexture(GL_TEXTURE1);
        gbuff->getTextures()[1]->bind();
        glActiveTexture(GL_TEXTURE2);
        gbuff->getTextures()[2]->bind();

        debugShader->setInt("gPosition", 0);
        debugShader->setInt("gNormal", 1);
        debugShader->setInt("gAlbedo", 2);

        debugShader->setInt("tonemap", 1);

        debugShader->setInt("show", 0);
        draw_quad({0.5, 0.5}, {0.5, 0.5});

        debugShader->setInt("show", 1);
        draw_quad({0.5, 0.0}, {0.5, 0.5});

        debugShader->setInt("show", 2);
        draw_quad({0.5, -0.5}, {0.5, 0.5});

        glActiveTexture(GL_TEXTURE0);
        glDisable(GL_TEXTURE_2D);
    }

    defaultShader->unbind();

    // getGlError();

    window->pushGLStates();
    window->draw(*fps);
    window->popGLStates();
}

void Game::drawPass(const MaterialShader::Ptr & shader) const {
    drawModel(sphereModel, shader);
    drawModel(cubeModel, shader);
    drawModel(hallModel, shader);
}

void Game::drawModel(const Model::ConstPtr & model,
                     const MaterialShader::Ptr & shader) const {
    shader->setMat4("model", model->toMatrix());
    model->draw(shader);
}

#include <iostream>
using namespace std;

#include "Game.hpp"

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
        std::cout << "gl error " << err << std::endl;
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
    if (!font.loadFromFile(resManager.resourceAt("Questrial_Regular.ttf"))) {
        cerr << "Failed to load font" << endl;
        return false;
    }

    // During init, enable debug output
    glEnable              ( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( MessageCallback, 0 );

    gridVerts = genGridVerts(10);
    gridCols = genGridCols(10);

    menu = std::make_shared<Menu>(font, "Game");
    menu->setPosition(300, 300);
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
    camera->rotate({30, -70});
    camera->setFov(80);

    defaultShader = resManager.loadShader("shader/default.vs", "shader/default.fs");
    if (!defaultShader) {
        throw std::runtime_error("Failed to load default shader");
    }

    geometryShader = resManager.loadShader("shader/geom.vert", "shader/geom.frag");
    if (!geometryShader) {
        throw std::runtime_error("Failed to load texture shader");
    }

    debugShader = resManager.loadShader("shader/debug.vert", "shader/debug.frag");
    if (!debugShader) {
        throw std::runtime_error("Failed to load debug shader");
    }

    lightingShader = std::make_shared<MaterialShader>();
    if (!lightingShader->loadFromPath(resManager.resourceAt("shader/lighting.vert"),
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
        std::cout << "Cube model failed" << std::endl;
        throw std::runtime_error("Failed to load cube model");
    }

    sphereModel = resManager.loadModel("model/sphere.obj");
    if (!sphereModel) {
        throw std::runtime_error("Failed to load sphere model");
    }
    sphereModel->move({1, 2, 3});
    sphereModel->scale({0.1, 0.1, 0.1});

    texture = resManager.loadTexture("dev_texture_gray", "img/dev_texture_gray.png");
    if (!texture) {
        throw std::runtime_error("Failed to load dev texture");
    }

    fbuff = std::make_shared<FrameBuffer>(window->getSize());
    fbuff->addTexture(GL_COLOR_ATTACHMENT0);
    fbuff->addTexture(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
    fbuff->finalize();

    gbuff = std::make_shared<FrameBuffer>(window->getSize());
    gbuff->addTexture(GL_COLOR_ATTACHMENT0, GL_RGBA16F, GL_RGBA, GL_FLOAT);
    gbuff->addTexture(GL_COLOR_ATTACHMENT1, GL_RGBA16F, GL_RGBA, GL_FLOAT);
    gbuff->addTexture(GL_COLOR_ATTACHMENT2, GL_RGBA, GL_RGBA, GL_FLOAT);
    //gbuff->addTexture(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
    gbuff->finalize();
    gbuff->addDepthBuffer();

    SetMouseGrab(true);

    getGlError();
    return true;
}

void Game::onDestroy() { }

void Game::onKeyPressed(const sf::Event::KeyEvent & e) {
    switch (e.code) {
        case sf::Keyboard::Num2:
            doDrawTexture = !doDrawTexture;
            break;
        case sf::Keyboard::Num3:
            doDrawShading = !doDrawShading;
            break;
        default:
            GameBase::onKeyPressed(e);
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
    fbuff->setSize({e.width, e.height});
}

void Game::onUpdate(const sf::Time & delta) {
    float deltaS = delta.asSeconds();

    time += deltaS;
    sphereModel->setPosition({glm::cos(time) * 3, 2, glm::sin(time) * 3});
    //cubeModel->setRotation({time, time / 3, time / 7});
}

void Game::onDraw() const {

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glm::mat4 vp = camera->projMatrix() * camera->viewMatrix();

    geometryShader->bind();
    {
        gbuff->bind();
        texture->bind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        geometryShader->setMat4("mvp", vp);
        geometryShader->setMat4("model", cubeModel->modelMatrix());
        cubeModel->draw();

        texture->unbind();
        gbuff->unbind();
    }

    fbuff->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lightingShader->bind();
    if (true) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        texture->bind();

        lightingShader->setMat4("vp", vp);
        lightingShader->setVec3("viewPos", camera->getPosition());
        lightingShader->setUInt("nLights", 2);

        lightingShader->setVec3("lights[0].ambient", {0.1, 0.1, 0.1});
        lightingShader->setVec3("lights[0].diffuse", {0.8, 0.8, 0.8});
        lightingShader->setVec3("lights[0].specular", {1, 1, 1});
        lightingShader->setVec3("lights[0].position", sphereModel->getPosition());
        lightingShader->setVec3("lights[0].direction", {-1, -2, -3});
        lightingShader->setUInt("lights[0].type", 1);

        lightingShader->setFloat("lights[0].constant", 1.0);
        lightingShader->setFloat("lights[0].linear", 0.09);
        lightingShader->setFloat("lights[0].quadratic", 0.032);

        lightingShader->setVec3("lights[1].ambient", {0.1, 0.1, 0.1});
        lightingShader->setVec3("lights[1].diffuse", {0.8, 0.8, 0.8});
        lightingShader->setVec3("lights[1].specular", {1, 1, 1});
        lightingShader->setVec3("lights[1].position", sphereModel->getPosition() + glm::vec3(1, 0, 1));
        lightingShader->setVec3("lights[1].direction", {-1, -2, -3});
        lightingShader->setUInt("lights[1].type", 2);

        lightingShader->setFloat("lights[1].constant", 1.0);
        lightingShader->setFloat("lights[1].linear", 0.09);
        lightingShader->setFloat("lights[1].quadratic", 0.032);

        lightingShader->setFloat("lights[1].cutOff", glm::cos(glm::radians(12.5)));
        lightingShader->setFloat("lights[1].outerCutOff", glm::cos(glm::radians(15.0)));

        drawPass(vp, lightingShader);
    }

    fbuff->unbind();
    fbuff->blit(0, GL_COLOR_BUFFER_BIT);

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    defaultShader->bind();
    if (false) {
        glUniformMatrix4fv(defaultShader->uniformLocation("mvp"), 1, GL_FALSE, &vp[0][0]);

        draw_color_array(gridVerts, gridCols, GL_LINES);
    }

    debugShader->bind();
    {
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

        debugShader->setInt("show", 0);
        debugShader->setInt("tonemap", 1);

        draw_quad({0.5, 0.5}, {0.5, 0.5});

        glActiveTexture(GL_TEXTURE0);
        glDisable(GL_TEXTURE_2D);
    }

    defaultShader->unbind();

    //getGlError();
}

void Game::drawPass(glm::mat4 vp, const MaterialShader::Ptr & shader) const {
    drawModel(sphereModel, vp, shader);
    drawModel(cubeModel, vp, shader);
}

void Game::drawModel(const Model::ConstPtr & model, glm::mat4 vp, const MaterialShader::Ptr & shader) const {
    shader->setMat4("model", model->modelMatrix());
    model->draw(shader);
}

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

bool Game::onCreate() {
    if (!font.loadFromFile(resManager.resourceAt("Questrial_Regular.ttf"))) {
        cerr << "Failed to load font" << endl;
        return false;
    }

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

    textureShader = resManager.loadShader("shader/tex.vs", "shader/tex.fs");
    if (!textureShader) {
        throw std::runtime_error("Failed to load texture shader");
    }

    lightingShader = resManager.loadShader("shader/lighting.vs", "shader/lighting.fs");
    if (!lightingShader) {
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
}

void Game::onUpdate(const sf::Time & delta) {
    float deltaS = delta.asSeconds();

    time += deltaS;
    sphereModel->setPosition({glm::cos(time) * 3, 2, glm::sin(time) * 3});
    cubeModel->setRotation({time, 0, 0});
}

void Game::onDraw() const {

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glm::mat4 vp = camera->projMatrix() * camera->viewMatrix();

    monoShader->bind();
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        texture->bind();

        monoShader->setMat4("vp", vp);
        monoShader->setVec3("viewPos", camera->getPosition());
        monoShader->setUInt("nLights", 2);

        monoShader->setVec3("lights[0].ambient", {0.1, 0.1, 0.1});
        monoShader->setVec3("lights[0].diffuse", {0.8, 0.8, 0.8});
        monoShader->setVec3("lights[0].specular", {1, 1, 1});
        monoShader->setVec3("lights[0].position", sphereModel->getPosition());
        monoShader->setVec3("lights[0].direction", {-1, -2, -3});
        monoShader->setUInt("lights[0].type", 1);

        monoShader->setVec3("lights[1].ambient", {0.1, 0.1, 0.1});
        monoShader->setVec3("lights[1].diffuse", {0.8, 0.8, 0.8});
        monoShader->setVec3("lights[1].specular", {1, 1, 1});
        monoShader->setVec3("lights[1].position", sphereModel->getPosition() + glm::vec3(1, 0, 1));
        monoShader->setVec3("lights[1].direction", {-1, -2, -3});
        monoShader->setUInt("lights[1].type", 2);

        monoShader->setFloat("lights[0].constant", 1.0);
        monoShader->setFloat("lights[0].linear", 0.09);
        monoShader->setFloat("lights[0].quadratic", 0.032);

        monoShader->setFloat("lights[0].cutOff", glm::cos(glm::radians(12.5)));
        monoShader->setFloat("lights[0].outerCutOff", glm::cos(glm::radians(15.0)));

        drawPass(vp, monoShader);
    }

    defaultShader->bind();
    {
        glDisable(GL_BLEND);
        glUniformMatrix4fv(defaultShader->uniformLocation("mvp"), 1, GL_FALSE, &vp[0][0]);

        draw_color_array(gridVerts, gridCols, GL_LINES);
    }

    defaultShader->unbind();

    getGlError();
}

void Game::drawPass(glm::mat4 vp, const MaterialShader::Ptr & shader) const {
    drawModel(sphereModel, vp, shader);
    drawModel(cubeModel, vp, shader);
}

void Game::drawModel(const Model::ConstPtr & model, glm::mat4 vp, const MaterialShader::Ptr & shader) const {
    shader->setMat4("model", model->modelMatrix());
    model->draw(shader);
}

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

Game::Game(const sf::String & resPath) : GameBase(resPath) { }

Game::~Game() { }

bool Game::onCreate() {
    if (!font.loadFromFile(resPath("res://Questrial_Regular.ttf"))) {
        cerr << "Failed to load font" << endl;
        return false;
    }

    lastMouse = {window->getSize().x / 2, window->getSize().y / 2};
    sf::Mouse::setPosition(lastMouse, *window);

    gridVerts = genGridVerts(10);
    gridCols = genGridCols(10);

    menu = Menu::create(font);
    menu->setTitle("Game");
    menu->setPosition(300, 300);
    menu->addMenuItem("New", [&]() {
        menu->hide();
        window->setMouseCursorGrabbed(true);
        window->setMouseCursorVisible(false);
    });
    menu->addMenuItem("Load", []() {});
    menu->addMenuItem("Options", []() {});
    menu->addMenuItem("Exit", [&]() {
        window->close();
    });

    cam = Camera::create();
    cam->setScreenSize(window->getSize());
    cam->move(3, 2, 1);
    cam->setFov(80);

    defaultShader = loadShader("res://shader/default.vs", "res://shader/default.fs");
    if (!defaultShader) {
        throw std::runtime_error("Failed to load default shader");
    }

    textureShader = loadShader("res://shader/tex.vs", "res://shader/tex.fs");
    if (!textureShader) {
        throw std::runtime_error("Failed to load texture shader");
    }

    lightingShader = loadShader("res://shader/lighting.vs", "res://shader/lighting.fs");
    if (!lightingShader) {
        throw std::runtime_error("Failed to load lighting shader");
    }

    monoShader = loadShader("res://shader/mono.vs", "res://shader/mono.fs");
    if (!monoShader) {
        throw std::runtime_error("Failed to load mono shader");
    }

    cubeModel = loadModel("res://model/cube.obj");
    if (!cubeModel) {
        std::cout << "Cube model failed" << std::endl;
        throw std::runtime_error("Failed to load cube model");
    }

    sphereModel = loadModel("res://model/sphere.obj");
    if (!sphereModel) {
        throw std::runtime_error("Failed to load sphere model");
    }
    sphereModel->move(1, 2, 3);
    sphereModel->scale(0.1, 0.1, 0.1);

    texture = loadTexture("dev_texture_gray", "res://img/dev_texture_gray.png");
    if (!texture) {
        throw std::runtime_error("Failed to load dev texture");
    }

    getGlError();
    return true;
}

void Game::onDestroy() { }

void Game::onKeyPressed(const sf::Event::KeyEvent & e) {
    switch (e.code) {
        case sf::Keyboard::Escape:
            menu->show();
            window->setMouseCursorGrabbed(false);
            window->setMouseCursorVisible(true);
            break;
        case sf::Keyboard::Num1:
            doDrawLegacy = !doDrawLegacy;
            break;
        case sf::Keyboard::Num2:
            doDrawTexture = !doDrawTexture;
            break;
        case sf::Keyboard::Num3:
            doDrawShading = !doDrawShading;
            break;
    }
}

void Game::onKeyReleased(const sf::Event::KeyEvent & e) {

}

void Game::onMouseMove(const sf::Event::MouseMoveEvent & e) {
    menu->onMouseMove(e);
}

void Game::onMouseDown(const sf::Event::MouseButtonEvent & e) {
    menu->onMouseDown(e);
}

void Game::onMouseUp(const sf::Event::MouseButtonEvent & e) {
    menu->onMouseUp(e);

    if (e.button == sf::Mouse::Button::Left) {
        // std::cout << "Camera Projection Matrix" << std::endl;
        // glm::mat4 mat = cam->projMatrix();
        // std::cout << mat << std::endl;

        // std::cout << "Camera View Matrix" << std::endl;
        // mat = cam->viewMatrix();
        // std::cout << mat << std::endl;
    }
}

void Game::onResized(const sf::Event::SizeEvent & e) {
    cam->setScreenSize(window->getSize());
    lastMouse = {window->getSize().x / 2, window->getSize().y / 2};
}

void Game::onUpdate(const sf::Time & delta) {
    float deltaS = delta.asSeconds();

    auto mPos = sf::Mouse::getPosition(*window);
    sf::Vector2f mDelta (mPos.x - lastMouse.x, mPos.y - lastMouse.y);
    if (!menu->isVisible) {
        sf::Mouse::setPosition(lastMouse, *window);
        cam->rotate(mDelta.y * 0.2, mDelta.x * 0.2);
    }

    if (!menu->isVisible) {
        int x = sf::Keyboard::isKeyPressed(sf::Keyboard::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        int y = sf::Keyboard::isKeyPressed(sf::Keyboard::E) - sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
        int z = sf::Keyboard::isKeyPressed(sf::Keyboard::S) - sf::Keyboard::isKeyPressed(sf::Keyboard::W);
        cam->moveLook(x * deltaS * 5, y * deltaS * 5, z * deltaS * 5);
    }

    time += deltaS;
    sphereModel->setPosition(glm::cos(time) * 3, 2, glm::sin(time) * 3);
    cubeModel->setRotation(time, 0, 0);
}

void Game::onDraw() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    modeProjection();
    float aspect = window->getSize().x / window->getSize().y;
    gluPerspective(93, aspect, 0.01, 100.0);
    modeModel();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glm::mat4 vp = cam->projMatrix() * cam->viewMatrix();

    monoShader->bind();
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        texture->bind();

        monoShader->setMat4("vp", vp);
        monoShader->setVec3("viewPos", cam->getPosition());
        monoShader->setUInt("nLights", 1);

        monoShader->setVec3("lights[0].ambient", 0.1, 0.1, 0.1);
        monoShader->setVec3("lights[0].diffuse", 0.8, 0.8, 0.8);
        monoShader->setVec3("lights[0].specular", 1, 1, 1);
        monoShader->setVec3("lights[0].position", sphereModel->getPosition());
        monoShader->setVec3("lights[0].direction", -1, -2, -3);
        monoShader->setUInt("lights[0].type", 1);

        monoShader->setVec3("lights[1].ambient", 0.1, 0.1, 0.1);
        monoShader->setVec3("lights[1].diffuse", 0.8, 0.8, 0.8);
        monoShader->setVec3("lights[1].specular", 1, 1, 1);
        monoShader->setVec3("lights[1].position", sphereModel->getPosition() + glm::vec3(1, 0, 1));
        monoShader->setVec3("lights[1].direction", -1, -2, -3);
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

    if (doDrawLegacy) {
        cam->pushTransform();
        draw_color_array_legacy(&gridVerts[0].x, &gridCols[0].x, gridVerts.size(), GL_LINES);
        cam->popTransform();
    }

    getGlError();

    window->pushGLStates();
    window->draw(*menu);
    window->popGLStates();
}

void Game::drawPass(glm::mat4 vp, const Shader::Ptr & shader) const {
    drawModel(sphereModel, vp, shader);
    drawModel(cubeModel, vp, shader);
}

void Game::drawModel(const Model::ConstPtr & model, glm::mat4 vp, const Shader::Ptr & shader) const {
    shader->setMat4("model", model->modelMatrix());

    const auto & m = model->getFirstMaterial();

    shader->setVec3("material.ambient", m->ambient);
    shader->setVec3("material.diffuse", m->diffuse);
    shader->setVec3("material.specular", m->specular);
    shader->setFloat("material.shininess", m->specularExponent);
    shader->setFloat("material.alpha", m->alpha);
    shader->setInt("material.texture", 0);

    model->draw();
}

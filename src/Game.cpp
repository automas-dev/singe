#include "Game.hpp"
#include "Util.hpp"

static void drawGrid(int steps = 10) {
    glBegin(GL_LINES);
    {
        glColor3f(1.0, 1.0, 1.0);
        for (int x = -steps; x <= steps; x++) {
            if (x == 0) {
                glVertex3f(0, 0, -steps);
                glVertex3f(0, 0, 0);
                glColor3f(0.0, 0.0, 1.0);
                glVertex3f(0, 0, 0);
                glVertex3f(0, 0, steps);
            }
            else {
                glColor3f(1.0, 1.0, 1.0);
                glVertex3f(x, 0, -steps);
                glVertex3f(x, 0, steps);
            }
        }
        for (int z = -steps; z <= steps; z++) {
            if (z == 0) {
                glVertex3f(-steps, 0, 0);
                glVertex3f(0, 0, 0);
                glColor3f(1.0, 0.0, 0.0);
                glVertex3f(0, 0, 0);
                glVertex3f(steps, 0, 0);
            }
            else {
                glColor3f(1.0, 1.0, 1.0);
                glVertex3f(-steps, 0, z);
                glVertex3f(steps, 0, z);
            }
        }
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, steps, 0);
    }
    glEnd();
}

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

namespace game {

    Game::Game(sf::RenderWindow & window, const sf::Font & defaultFont) : window(window), font(defaultFont) {
        lastMouse = {window.getSize().x / 2, window.getSize().y / 2};
        sf::Mouse::setPosition(lastMouse, window);

        gridVerts = genGridVerts(10);
        gridCols = genGridCols(10);

        menu = Menu::create(font);
        menu->setTitle("Game");
        menu->setPosition(300, 300);
        menu->addMenuItem("New", [&]() {
            menu->hide();
            window.setMouseCursorGrabbed(true);
            window.setMouseCursorVisible(false);
        });
        menu->addMenuItem("Load", []() {});
        menu->addMenuItem("Options", []() {});
        menu->addMenuItem("Exit", [&]() {
            window.close();
        });

        cam = Camera::create();
        cam->setScreen(window.getSize());
        cam->move(3, 2, 1);
        cam->setFov(80);

        defaultShader = Shader::create("res/shader/color.vs", "res/shader/color.fs");
        if (!defaultShader) {
            throw std::runtime_error("Failed to load default shader");
        }

        objShader = Shader::create("res/shader/obj.vs", "res/shader/obj.fs");
        if (!objShader) {
            throw std::runtime_error("Failed to load obj shader");
        }

        objUniforms.loadFromShader(objShader);

        gridModel = Model::create("res/model/grid.obj");
        if (!gridModel) {
            std::cout << "Grid model failed" << std::endl;
            throw std::runtime_error("Failed to load grid model");
        }

        texture = Texture::create("res/img/dev_texture_gray.png");
        if (!texture) {
            std::cout << "Texture failed to load" << std::endl;
            throw std::runtime_error("Failed to load texture");
        }
    }

    Game::~Game() { }

    void Game::onKeyPressed(const sf::Event::KeyEvent & e) {
        switch (e.code) {
            case sf::Keyboard::Escape:
                menu->show();
                window.setMouseCursorGrabbed(false);
                window.setMouseCursorVisible(true);
                break;
            case sf::Keyboard::Num1:
                doDrawLegacy = !doDrawLegacy;
                break;
            case sf::Keyboard::Num2:
                doDrawMatrix = !doDrawMatrix;
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
            std::cout << "Camera Projection Matrix" << std::endl;
            glm::mat4 mat = cam->projMatrix();
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    std::cout << mat[i][j] << " ";
                }
                std::cout << std::endl;
            }

            std::cout << "Camera View Matrix" << std::endl;
            mat = cam->viewMatrix();
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    std::cout << mat[i][j] << " ";
                }
                std::cout << std::endl;
            }
        }
    }

    void Game::onResized(const sf::Event::SizeEvent & e) {
        cam->setScreen(window.getSize());
        lastMouse = {window.getSize().x / 2, window.getSize().y / 2};
    }

    void Game::update(const sf::Time & delta) {
        float deltaS = delta.asSeconds();

        auto mPos = sf::Mouse::getPosition(window);
        sf::Vector2f mDelta (mPos.x - lastMouse.x, mPos.y - lastMouse.y);
        if (!menu->isVisible) {
            sf::Mouse::setPosition(lastMouse, window);
            cam->rotate(mDelta.y * 0.2, mDelta.x * 0.2);
        }

        if (!menu->isVisible) {
            int x = sf::Keyboard::isKeyPressed(sf::Keyboard::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::A);
            int y = sf::Keyboard::isKeyPressed(sf::Keyboard::E) - sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
            int z = sf::Keyboard::isKeyPressed(sf::Keyboard::S) - sf::Keyboard::isKeyPressed(sf::Keyboard::W);
            cam->moveLook(x * deltaS, y * deltaS, z * deltaS);
        }
    }

    void Game::draw() const {
        glDisable(GL_CULL_FACE);

        // glEnable(GL_TEXTURE_2D);

        defaultShader->bind();

        glm::mat4 mvp = cam->projMatrix() * cam->viewMatrix();
        glUniformMatrix4fv(defaultShader->uniformLocation("mvp"), 1, GL_FALSE, &mvp[0][0]);

        // texture->bind();
        // cam->pushTransform();

        // glUniformMatrix4fv(objShader->uniformLocation("proj"), 1, GL_FALSE, &cam->projMatrix()[0][0]);
        // glUniformMatrix4fv(objShader->uniformLocation("view"), 1, GL_FALSE, &cam->viewMatrix()[0][0]);
        // glUniformMatrix4fv(objShader->uniformLocation("model"), 1, GL_FALSE, &gridModel->modelMatrix()[0][0]);

        // glUniform3f(objShader->uniformLocation("lightPos"), 1, 2, 3);
        // glUniform3fv(objShader->uniformLocation("viewPos"), 1, &cam->getPosition()[0]);

        // glUniform3f(objShader->uniformLocation("ambient"), 1, 1, 1);
        // glUniform3f(objShader->uniformLocation("diffuse"), .5, .5, .5);
        // glUniform3f(objShader->uniformLocation("specular"), .1, .1, .1);

        // glUniform1f(objShader->uniformLocation("specExp"), 1);
        // glUniform1f(objShader->uniformLocation("alpha"), 1);

        // gridModel->draw();

        if (doDrawMatrix) {
            // draw_color_array(&gridVerts[0].x, &gridCols[0].x, gridVerts.size(), GL_LINES);
            draw_two_array(gridVerts, gridCols, GL_LINES);
        }

        // cam->popTransform();

        // glDisable(GL_TEXTURE_2D);
        defaultShader->unbind();

        if (doDrawLegacy) {
            cam->pushTransform();
            // drawGrid(10);
            draw_color_array(&gridVerts[0].x, &gridCols[0].x, gridVerts.size(), GL_LINES);
            cam->popTransform();
        }

        window.pushGLStates();
        window.draw(*menu);
        window.popGLStates();
    }

    Game::Ptr Game::create(sf::RenderWindow & window, const sf::Font & defaultFont) {
        auto game = std::make_shared<Game>(window, defaultFont);
        return game;
    }
}

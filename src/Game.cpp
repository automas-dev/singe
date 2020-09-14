#include "Game.hpp"

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

namespace game {

    Game::Game(sf::RenderWindow & window, const sf::Font & defaultFont) : window(window), font(defaultFont) {
        lastMouse = {window.getSize().x / 2, window.getSize().y / 2};
        sf::Mouse::setPosition(lastMouse, window);

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
    }

    Game::~Game() { }

    void Game::onKeyPressed(const sf::Event::KeyEvent & e) {
        switch (e.code) {
            case sf::Keyboard::Escape:
                menu->show();
                window.setMouseCursorGrabbed(false);
                window.setMouseCursorVisible(true);
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

    void Game::draw() {
        cam->pushTransform();
        drawGrid(20);
        cam->popTransform();

        window.pushGLStates();
        window.draw(*menu);
        window.popGLStates();
    }

    Game::Ptr Game::create(sf::RenderWindow & window, const sf::Font & defaultFont) {
        auto game = std::make_shared<Game>(window, defaultFont);
        return game;
    }
}

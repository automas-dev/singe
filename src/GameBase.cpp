#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#define GLEW_STATIC
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include "s3e/GameBase.hpp"

namespace Tom::s3e {

    GameBase::GameBase(const sf::String & resPath) : ResourceManager(resPath) { }

    GameBase::~GameBase() { }

    bool GameBase::Create(const std::string & title, unsigned int width, unsigned int height, bool fullscreen) {
        sf::ContextSettings settings;
        settings.depthBits = 24;
        settings.antialiasingLevel = 8;
        settings.majorVersion = 3;
        settings.minorVersion = 0;

        window = std::make_shared<sf::RenderWindow>(
                     sf::VideoMode(width, height),
                     title,
                     sf::Style::Default | (fullscreen ? sf::Style::Fullscreen : 0),
                     settings);

        window->setVerticalSyncEnabled(true);
        window->setFramerateLimit(60);
        window->setActive();
        window->setKeyRepeatEnabled(false);

        // glewExperimental = true;
        GLenum err = glewInit();
        if (err != GLEW_OK) {
            std::cerr << "glewInit failed: " << glewGetErrorString(err) << std::endl;
            return false;
        }

        return onCreate();
    }

    void GameBase::Start(void) {
        sf::Clock clock;

        while (window->isOpen()) {
            sf::Event event;
            while (window->pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::KeyPressed:
                        onKeyPressed(event.key);
                        break;
                    case sf::Event::KeyReleased:
                        onKeyReleased(event.key);
                        break;
                    case sf::Event::MouseMoved:
                        onMouseMove(event.mouseMove);
                        break;
                    case sf::Event::MouseButtonPressed:
                        onMouseDown(event.mouseButton);
                        break;
                    case sf::Event::MouseButtonReleased:
                        onMouseUp(event.mouseButton);
                        break;
                    case sf::Event::MouseWheelScrolled:
                        onMouseScroll(event.mouseWheelScroll);
                        break;
                    case sf::Event::Resized: {
                            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                            window->setView(sf::View(visibleArea));
                            glViewport(0, 0, event.size.width, event.size.height);
                            onResized(event.size);
                        }
                        break;
                    case sf::Event::Closed:
                        Stop();
                        break;
                }
            }

            sf::Time delta = clock.restart();

            onUpdate(delta);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            onDraw();
            window->display();
        }

        onDestroy();
    }

    void GameBase::Stop(void) {
        window->close();
    }

    void GameBase::Fail(int status) noexcept {
        window->close();
        exit(status);
    }

    void GameBase::SetMouseGrab(bool grab) {
        window->setMouseCursorGrabbed(grab);
        window->setMouseCursorVisible(!grab);
    }

    void GameBase::onKeyPressed(const sf::Event::KeyEvent & event) {
        if (event.code == sf::Keyboard::Escape)
            Stop();
    }

    void GameBase::onKeyReleased(const sf::Event::KeyEvent & event) { }

    void GameBase::onMouseMove(const sf::Event::MouseMoveEvent & event) { }

    void GameBase::onMouseDown(const sf::Event::MouseButtonEvent & event) { }

    void GameBase::onMouseUp(const sf::Event::MouseButtonEvent & event) { }

    void GameBase::onMouseScroll(const sf::Event::MouseWheelScrollEvent & event) { }

    void GameBase::onResized(const sf::Event::SizeEvent & event) { }
}

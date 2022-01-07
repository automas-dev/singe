#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#define GLEW_STATIC
#include <GL/glew.h>

#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <glpp/FrameBuffer.hpp>

#include "default_font.h"
#include "singe/Core/GameBase.hpp"

namespace singe::Logging {
    Logger::Ptr Game = std::make_shared<Logger>("Game");
}

namespace singe {

    GameBase::GameBase(Window & window)
        : window(window),
          mouseSensitivity(0.2, 0.2),
          moveSpeed(5),
          camera(nullptr),
          menu(nullptr),
          defaultShader(Shader::defaultShader()) {

        camera = std::make_shared<Camera>();
        auto size = window.getSize();
        camera->setScreenSize({size.x, size.y});

        uiFont.loadFromMemory(__default_font_start, __default_font_size);

        // menu = std::make_shared<Menu>(uiFont, title);
        // menu->setPosition(300, 300);
        // window.addEventHandler(menu.get());

        window.addEventHandler(this);
    }

    GameBase::~GameBase() {}

    void GameBase::Start(void) {
        Logging::Core->info("starting main game loop");

        sf::Clock clock;

        while (window.isOpen()) {
            window.poll();

            sf::Time delta = clock.restart();

            // if (grab) {
            int x = sf::Keyboard::isKeyPressed(sf::Keyboard::D)
                    - sf::Keyboard::isKeyPressed(sf::Keyboard::A);
            int y = sf::Keyboard::isKeyPressed(sf::Keyboard::E)
                    - sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
            int z = sf::Keyboard::isKeyPressed(sf::Keyboard::S)
                    - sf::Keyboard::isKeyPressed(sf::Keyboard::W);

            glm::ivec2 center(window.getSize().x / 2, window.getSize().y / 2);
            auto mouse = window.getMousePosition();
            if (window.getMouseGrab()) {
                window.setMousePosition(center);
            }

            glm::vec2 mouseDelta(mouse.x - center.x, mouse.y - center.y);
            mouseDelta *= mouseSensitivity;
            glm::vec3 rotation(glm::radians(mouseDelta.y),
                               glm::radians(mouseDelta.x),
                               0);
            if (rotation.x != 0 || rotation.y != 0)
                camera->rotateEuler(rotation);

            camera->moveDolly({x * delta.asSeconds() * moveSpeed,
                               y * delta.asSeconds() * moveSpeed,
                               z * delta.asSeconds() * moveSpeed});
            // }

            onUpdate(delta);

            FrameBuffer::clear();
            defaultShader.bind();
            onDraw();
            // if (menu) {
            //     window->pushGLStates();
            //     window->draw(*menu);
            //     window->popGLStates();
            // }
            window.display();
        }
    }

    void GameBase::Stop(void) {
        Logging::Core->info("stopping main game loop");
        window.close();
    }

    void GameBase::Fail(int status) noexcept {
        Logging::Core->error("a failure occurred, status={}, exiting!", status);
        window.close();
        exit(status);
    }

    void GameBase::SetMouseSensitivity(const glm::vec2 & sensitivity) {
        mouseSensitivity = sensitivity;
    }

    void GameBase::SetMoveSpeed(float speed) {
        moveSpeed = speed;
    }

    void GameBase::onKeyPressed(const sf::Event::KeyEvent & event) {
        if (event.code == sf::Keyboard::Escape) {
            window.setMouseGrab(!window.getMouseGrab());
            // if (menu->isVisible()) {
            //     menu->hide();
            //     SetMouseGrab(true);
            // }
            // else {
            //     menu->show();
            //     SetMouseGrab(false);
            // }
        }
    }

    void GameBase::onResized(const sf::Event::SizeEvent & event) {
        camera->setScreenSize({event.width, event.height});
    }
}

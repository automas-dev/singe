#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#define GLEW_STATIC
#include <GL/glew.h>

#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>

#include "default_font.h"
#include "s3e/Core/GameBase.hpp"
#include "s3e/Support/log.hpp"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Tom::s3e {

    GameBase::GameBase()
        : grab(false),
          mouseSensitivity(0.2, 0.2),
          moveSpeed(5),
          camera(nullptr),
          menu(nullptr) {}

    GameBase::~GameBase() {}

    bool GameBase::Create(const std::string & title,
                          unsigned int width,
                          unsigned int height,
                          bool fullscreen) {

        Logging::Core->info(
            "creating game title=\"{}\" width={} height={} fullscreen={}",
            title,
            width,
            height,
            fullscreen);

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

        Logging::Core->debug("initializing GLEW");
        // glewExperimental = true;
        GLenum err = glewInit();
        if (err != GLEW_OK) {
            Logging::Core->error("glewInit failed: {}", glewGetErrorString(err));
            return false;
        }

        camera = std::make_shared<Camera>();
        camera->setScreenSize(window->getSize());

        uiFont.loadFromMemory(__default_font_start, __default_font_size);

        menu = std::make_shared<Menu>(uiFont, title);
        menu->setPosition(300, 300);

        defaultShader = Shader::defaultShader();

        Logging::Core->debug("calling onCreate()");
        bool res = onCreate();
        if (!res)
            Logging::Core->error("User overridden onCreate returned false");
        return res;
    }

    void GameBase::Start(void) {
        Logging::Core->info("starting main game loop");

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
                        Logging::Core->debug("window resized to {} x {}",
                                             event.size.width,
                                             event.size.height);
                        sf::FloatRect visibleArea(0,
                                                  0,
                                                  event.size.width,
                                                  event.size.height);
                        window->setView(sf::View(visibleArea));
                        glViewport(0, 0, event.size.width, event.size.height);
                        onResized(event.size);
                    } break;
                    case sf::Event::Closed:
                        Stop();
                        break;
                    default:
                        break;
                }
            }

            sf::Time delta = clock.restart();

            if (grab) {
                int x = sf::Keyboard::isKeyPressed(sf::Keyboard::D)
                        - sf::Keyboard::isKeyPressed(sf::Keyboard::A);
                int y = sf::Keyboard::isKeyPressed(sf::Keyboard::E)
                        - sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
                int z = sf::Keyboard::isKeyPressed(sf::Keyboard::S)
                        - sf::Keyboard::isKeyPressed(sf::Keyboard::W);

                sf::Vector2i center(window->getSize().x / 2,
                                    window->getSize().y / 2);
                sf::Vector2i mouse = sf::Mouse::getPosition(*window);
                sf::Mouse::setPosition(center, *window);

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
            }

            onUpdate(delta);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            defaultShader->bind();
            onDraw();
            if (menu) {
                window->pushGLStates();
                window->draw(*menu);
                window->popGLStates();
            }
            window->display();
        }

        onDestroy();
    }

    void GameBase::Stop(void) {
        Logging::Core->info("stopping main game loop");
        window->close();
    }

    void GameBase::Fail(int status) noexcept {
        Logging::Core->error("a failure occurred, status={}, exiting!", status);
        window->close();
        exit(status);
    }

    void GameBase::SetMouseGrab(bool grab) {
        Logging::Core->debug("mouse grab set to {}", grab);
        this->grab = grab;

        window->setMouseCursorVisible(!grab);

        if (grab) {
            auto size = window->getSize();
            sf::Vector2i center(size.x / 2, size.y / 2);
            sf::Mouse::setPosition(center, *window);
        }
    }

    void GameBase::SetMouseSensitivity(const glm::vec2 & sensitivity) {
        mouseSensitivity = sensitivity;
    }

    void GameBase::SetMoveSpeed(float speed) {
        moveSpeed = speed;
    }

    void GameBase::onKeyPressed(const sf::Event::KeyEvent & event) {
        if (event.code == sf::Keyboard::Escape) {
            if (menu->isVisible()) {
                menu->hide();
                SetMouseGrab(true);
            }
            else {
                menu->show();
                SetMouseGrab(false);
            }
        }
    }

    void GameBase::onKeyReleased(const sf::Event::KeyEvent & event) {}

    void GameBase::onMouseMove(const sf::Event::MouseMoveEvent & event) {
        if (menu)
            menu->onMouseMove(event);
    }

    void GameBase::onMouseDown(const sf::Event::MouseButtonEvent & event) {
        if (menu)
            menu->onMouseDown(event);
    }

    void GameBase::onMouseUp(const sf::Event::MouseButtonEvent & event) {
        if (menu)
            menu->onMouseUp(event);
    }

    void GameBase::onMouseScroll(const sf::Event::MouseWheelScrollEvent & event) {}

    void GameBase::onResized(const sf::Event::SizeEvent & event) {
        camera->setScreenSize({event.width, event.height});
    }
}

#include "singe/Core/Window.hpp"

namespace singe {
    const sf::ContextSettings settings(24, 1, 8, 3, 0);

    Window::Window(const std::string & title,
                   unsigned int width,
                   unsigned int height,
                   bool fullscreen)
        : window(sf::VideoMode(width, height),
                 title,
                 sf::Style::Default | (fullscreen ? sf::Style::Fullscreen : 0),
                 settings) {
        window.setVerticalSyncEnabled(true);
        window.setFramerateLimit(60);
        window.setActive();
        window.setKeyRepeatEnabled(false);
    }

    Window::~Window() {
        window.close();
    }

    void Window::setMouseGrab(bool grab) {
        this->grab = grab;
        window.setMouseCursorVisible(!grab);

        if (grab) {
            auto size = window.getSize();
            sf::Vector2i center(size.x / 2, size.y / 2);
            sf::Mouse::setPosition(center, window);
        }
    }

    glm::ivec2 Window::getMousePosition() const {
        auto pos = sf::Mouse::getPosition(window);
        return {pos.x, pos.y};
    }

    void Window::setMousePosition(const glm::ivec2 & pos) {
        sf::Mouse::setPosition({pos.x, pos.y});
    }

    bool Window::isOpen() const {
        return window.isOpen();
    }

    void Window::close() {
        window.close();
    }

    glm::uvec2 Window::getSize() const {
        auto size = window.getSize();
        return {size.x, size.y};
    }

#define FIRE_EVENT(E) \
    for (auto & handler : handlers) handler->E;

    void Window::poll() {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::KeyPressed:
                    FIRE_EVENT(onKeyPressed(event.key));
                    break;
                case sf::Event::KeyReleased:
                    FIRE_EVENT(onKeyReleased(event.key));
                    break;
                case sf::Event::MouseMoved:
                    FIRE_EVENT(onMouseMove(event.mouseMove));
                    break;
                case sf::Event::MouseButtonPressed:
                    FIRE_EVENT(onMouseDown(event.mouseButton));
                    break;
                case sf::Event::MouseButtonReleased:
                    FIRE_EVENT(onMouseUp(event.mouseButton));
                    break;
                case sf::Event::MouseWheelScrolled:
                    FIRE_EVENT(onMouseScroll(event.mouseWheelScroll));
                    break;
                case sf::Event::Resized: {
                    sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                    window.setView(sf::View(visibleArea));
                    glViewport(0, 0, event.size.width, event.size.height);
                    FIRE_EVENT(onResized(event.size));
                } break;
                default:
                    break;
            }
        }
    }

#undef FIRE_EVENT

    void Window::display() {
        window.display();
    }

    void Window::addEventHandler(EventHandler * handler) {
        handlers.push_back(handler);
    }
}

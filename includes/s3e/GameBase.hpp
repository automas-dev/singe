#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "Camera.hpp"
#include "Menu.hpp"
#include "ResourceManager.hpp"

namespace Tom::s3e {

    class GameBase : public ResourceManager {
        sf::Vector2i lastMouse;

    protected:
        std::shared_ptr<sf::RenderWindow> window;

    public:
        GameBase(const sf::String & resPath);
        virtual ~GameBase();

        bool Create(const std::string & title, unsigned int width = 800, unsigned int height = 600, bool fullscreen=false);
        void Start(void);
        void Stop(void);
        void Fail(int status = 1) noexcept;

    protected:
        virtual bool onCreate(void) = 0;
        virtual void onDestroy(void) = 0;
        virtual void onUpdate(const sf::Time & delta) = 0;
        virtual void onDraw() const = 0;

        virtual void onKeyPressed(const sf::Event::KeyEvent & event);
        virtual void onKeyReleased(const sf::Event::KeyEvent & event);
        virtual void onMouseMove(const sf::Event::MouseMoveEvent & event);
        virtual void onMouseDown(const sf::Event::MouseButtonEvent & event);
        virtual void onMouseUp(const sf::Event::MouseButtonEvent & event);
        virtual void onMouseScroll(const sf::Event::MouseWheelScrollEvent & event);
        virtual void onResized(const sf::Event::SizeEvent & event);
    };
}

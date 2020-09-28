#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <memory>
#include "Menu.hpp"
#include "Camera.hpp"

namespace game {

    class Game : public std::enable_shared_from_this<Game> {
        sf::RenderWindow & window;

        sf::Font font;
        Menu::Ptr menu;
        Camera::Ptr cam;

        sf::Vector2i lastMouse;

    public:
        typedef std::shared_ptr<Game> Ptr;
        typedef std::shared_ptr<const Game> ConstPtr;

        Game(sf::RenderWindow & window, const sf::Font & defaultFont);
        virtual ~Game();

        void onKeyPressed(const sf::Event::KeyEvent & e);
        void onKeyReleased(const sf::Event::KeyEvent & e);

        void onMouseMove(const sf::Event::MouseMoveEvent & e);
        void onMouseDown(const sf::Event::MouseButtonEvent & e);
        void onMouseUp(const sf::Event::MouseButtonEvent & e);

        void onResized(const sf::Event::SizeEvent & e);

        void update(const sf::Time & delta);
        void draw() const;

        static Ptr create(sf::RenderWindow & window, const sf::Font & defaultFont);
    };
}

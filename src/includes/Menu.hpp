#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <list>

namespace game {
    class MenuItem : public sf::Text {
        std::function<void(void)> callback;

    public:
        MenuItem(void);
        virtual ~MenuItem();

        void setCallback(std::function<void(void)> callback);

        void click();

        bool contains(sf::Vector2f point);
    };

    class Menu : public sf::Drawable, public sf::Transformable {
        sf::Font font;
        sf::Text title;
        std::list<MenuItem> items;
        bool isMouseDown = false;

    public:
        bool isVisible = false;

        Menu(sf::Font font);
        virtual ~Menu();

        void setTitle(std::string text);

        void show();

        void hide();

        void addMenuItem(std::string text, std::function<void(void)> callback);

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

        void onMouseMove(sf::Event::MouseMoveEvent e);

        void onMouseDown(sf::Event::MouseButtonEvent e);

        void onMouseUp(sf::Event::MouseButtonEvent e);
    };
}

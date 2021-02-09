#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <list>
#include <memory>

namespace Tom::s3e {

    class MenuItem : public sf::Text {
        std::function<void(void)> callback;

    public:
        typedef std::shared_ptr<MenuItem> Ptr;
        typedef std::shared_ptr<const MenuItem> ConstPtr;

        MenuItem(void);
        virtual ~MenuItem();

        void setCallback(std::function<void(void)> callback);

        void onClick() const;

        bool contains(sf::Vector2f point) const;
    };

    /**
     * A menu system for the title screen and the in game pause menu.
     */
    class Menu : public sf::Drawable, public sf::Transformable {
        sf::Font font;
        sf::Text title;
        std::list<MenuItem::Ptr> items;
        bool isMouseDown = false;
        bool visible = false;

    public:
        /**
         * A shared pointer that manages a Menu.
         */
        typedef std::shared_ptr<Menu> Ptr;

        /**
         * A shared pointer that manages a const Menu.
         */
        typedef std::shared_ptr<const Menu> ConstPtr;

        /**
         * Construct a Menu.
         */
        Menu(void);

        /**
         * Construct a Menu and use `font` for the title and menu items.
         *
         * @param font the default font for the Menu
         */
        Menu(const sf::Font & font);

        /**
         * Construct a Menu, use `font` for the title and menu items and set
         * the title to `title`.
         *
         * @param font the Font to use for the title and menu items
         * @param title the menu title
         */
        Menu(const sf::Font & font, const std::string & title);

        /**
         * Destruct the Menu.
         */
        virtual ~Menu();

        /**
         * Set the Font to be used by the menu title and menu items.
         *
         * @param font the Font to be used for the title and menu items
         */
        void setFont(const sf::Font & font);

        /**
         * Set the menu title. This is displayed above the menu items.
         *
         * @param text the new title
         */
        void setTitle(const std::string & text);

        /**
         * Make the Menu visible and start accepting events.
         */
        void show(void);

        /**
         * Hide the Menu and stop responding to events.
         */
        void hide(void);

        /**
         * Is the Menu currently visible.
         *
         * @return is the Menu visible
         */
        bool isVisible(void);

        // TODO: Document addMenuItem once it's fate is decided
        bool addMenuItem(const std::string & text, std::function<void(void)> callback);

        /**
         * Override for sf::Drawable::draw. If the menu is visible, this method
         * will draw a background, the title and the menu items.
         *
         * @param target the sf::RenderTarget to draw to
         * @param states the current sf::RenderStates used to draw the title and
         *               menu items
         */
        void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

        /**
         * Event callback for the mouse move event. If the menu is not visible,
         * this method will ignore the event.
         *
         * @param e the mouse move event
         */
        void onMouseMove(sf::Event::MouseMoveEvent e);

        /**
         * Event callback for the mouse down event. If the menu is not visible,
         * this method will ignore the event.
         *
         * @param e the mouse press event
         */
        void onMouseDown(sf::Event::MouseButtonEvent e);

        /**
         * Event callback for the mouse up event. If the menu is not visible,
         * this method will ignore the event.
         *
         * @param e the mouse up event
         */
        void onMouseUp(sf::Event::MouseButtonEvent e);
    };
}


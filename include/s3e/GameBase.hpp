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

    /**
     * Game base to be extended by the user. This class manages the window,
     * main game loop, event callbacks, resource loading, and mouse capture.
     *
     * The user must provide the onCreate, onDestroy, onUpdate and onDraw
     * methods.
     *
     * The user may optionally override the onKeyPressed, onKeyReleased,
     * onMouseMove, onMouseDown, onMouseUp, onMouseScroll or onResized.
     */
    class GameBase : public ResourceManager {
        sf::Vector2i lastMouse;

    protected:
        std::shared_ptr<sf::RenderWindow> window;

    public:

        /**
         * Construct a new GameBase.
         */
        GameBase(const sf::String & resPath);

        /**
         * Destruct the GameBase.
         */
        virtual ~GameBase();

        /**
         * Create the window and load resources. This method calls the
         * onCreate() virtual method.
         *
         * @param title the window title
         * @param width the window width in pixels
         * @param height the window height in pixels
         * @param fullscreen should the window start in fullscreen
         *
         * @return was the window created and the call to onCreate() returns true
         */
        bool Create(const std::string & title,
                    unsigned int width = 800,
                    unsigned int height = 600,
                    bool fullscreen = false);

        /**
         * Start the main game loop. This method blocks until the game is
         * stopped.
         */
        void Start(void);

        /**
         * Stop the game, close the window and free resources. The onDestroy()
         * method is called after the game loop ends.
         */
        void Stop(void);

        /**
         * Abort by calling exit() with status.
         *
         * @param status the exist status
         */
        void Fail(int status = 1) noexcept;


        /**
         * Grab or release the mouse.
         *
         * @param grab should the mouse be grabbed or released
         */
        void SetMouseGrab(bool grab);

        // TODO: Implement GameBase::SetFullscreen()
        void SetFullscreen(bool fullscreen);

    protected:

        /**
         * Load and initialize any resources required by the game. This method
         * is called in Start() after the window is created. The return from
         * this method is returned by Start()
         *
         * @return was initialization successful
         */
        virtual bool onCreate(void) = 0;

        /**
         * Free any resources used by the game. This method is called after the
         * main game loop stops.
         */
        virtual void onDestroy(void) = 0;

        /**
         * Process any updates before drawing the next frame.
         *
         * @param delta the time since the last call to onUpdate()
         */
        virtual void onUpdate(const sf::Time & delta) = 0;

        /**
         * Draw the frame.
         */
        virtual void onDraw() const = 0;

        /**
         * Event callback for a key press event.
         *
         * @param event the key press event
         */
        virtual void onKeyPressed(const sf::Event::KeyEvent & event);

        /**
         * Event callback for a key release event
         *
         * @param event the key release event
         */
        virtual void onKeyReleased(const sf::Event::KeyEvent & event);

        /**
         * Event callback for a mouse move event.
         *
         * @param event the mouse move event
         */
        virtual void onMouseMove(const sf::Event::MouseMoveEvent & event);

        /**
         * Event callback for a mouse button press.
         *
         * @param event the mouse button event
         */
        virtual void onMouseDown(const sf::Event::MouseButtonEvent & event);

        /**
         * Event callback for a mouse button release.
         *
         * @param event the mouse button event
         */
        virtual void onMouseUp(const sf::Event::MouseButtonEvent & event);

        /**
         * Event callback for a mouse scroll event.
         *
         * @param event the mouse scroll event
         */
        virtual void onMouseScroll(const sf::Event::MouseWheelScrollEvent & event);

        /**
         * Event callback for a window resize event.
         *
         * @param event the window resize event
         */
        virtual void onResized(const sf::Event::SizeEvent & event);
    };
}

#pragma once

#include <GL/glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "s3e/Core/Camera.hpp"
#include "s3e/Core/Menu.hpp"
#include "s3e/Graphics/Shader.hpp"

namespace Tom::s3e {

    /**
     * Game base to be extended by the user. This class manages the window,
     * main game loop, event callbacks and mouse capture.
     *
     * The user must provide the onCreate, onDestroy, onUpdate and onDraw
     * methods.
     *
     * The user may optionally override the onKeyPressed, onKeyReleased,
     * onMouseMove, onMouseDown, onMouseUp, onMouseScroll or onResized.
     */
    class GameBase {
        bool grab;
        glm::vec2 mouseSensitivity;
        float moveSpeed;

    protected:
        /// Pointer to the SFML RenderWindow object
        std::shared_ptr<sf::RenderWindow> window;
        /**
         * Pointer to the Camera object created by GameBase.
         *
         * This pointer may be replaced by a derived class but it must exist.
         */
        Camera::Ptr camera;
        /**
         * Pointer to the Menu object created by GameBase.
         *
         * This pointer may be replaced or set to nullptr by a derived class.
         */
        Menu::Ptr menu;
        /// A font loaded from memory to act as the default font for GameBase.
        sf::Font uiFont;
        /// Pointer to a shader that is bound before the call to onDraw().
        Shader::Ptr defaultShader;

    public:
        /**
         * Construct a new GameBase.
         */
        GameBase(void);

        /**
         * Destruct the GameBase.
         */
        virtual ~GameBase();

        /**
         * Create the window. This method calls the onCreate() virtual method
         * which is where the user should load and initialize resources before
         * the game starts.
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
         * Stop the game and close the window. The onDestroy() method is called
         * after the game loop ends.
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

        /**
         * Set the mouse sensitivity for the x and y axis.
         *
         * @param sensitivity the new mouse sensitivity
         */
        void SetMouseSensitivity(const glm::vec2 & sensitivity);

        /**
         * Set the move speed for the camera.
         *
         * @param speed the new move speed
         */
        void SetMoveSpeed(float speed);

    protected:
        /**
         * Load and initialize any resources Used by the game. This method is
         * called in Start() after the window is created. The return from this
         * method is returned by Start()
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

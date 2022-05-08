#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glpp/Shader.hpp>
#include <glpp/extra/Camera.hpp>
#include <memory>
#include <singe/Support/log.hpp>
#include <vector>

#include "singe/Core/FPSDisplay.hpp"
#include "singe/Core/Menu.hpp"
#include "singe/Core/Window.hpp"

namespace singe::Logging {
    extern Logger::Ptr Game;
}

namespace singe {
    using namespace glpp;
    using namespace glpp::extra;

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
    class GameBase : public EventHandler {
        glm::vec2 mouseSensitivity;
        float moveSpeed;
        bool fpsShow;

    protected:
        /// Reference to the Window object
        Window & window;

        /**
         * Pointer to the Camera object created by GameBase.
         *
         * This pointer may be replaced by a derived class but it must exist.
         */
        Camera camera;

        FPSDisplay fpsDisplay;

        /**
         * Pointer to the Menu object created by GameBase.
         *
         * This pointer may be replaced or set to nullptr by a derived class.
         */
        Menu::Ptr menu;

        /// A font loaded from memory to act as the default font for GameBase.
        sf::Font uiFont;

        /// Pointer to a shader that is bound before the call to onDraw().
        Shader defaultShader;

    public:
        /**
         * Construct a new GameBase.
         */
        GameBase(Window & window);

        /**
         * Destruct the GameBase.
         */
        virtual ~GameBase();

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

        void showFps();

        void hideFps();

    protected:
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
        void onKeyPressed(const sf::Event::KeyEvent & event) override;

        /**
         * Event callback for a window resize event.
         *
         * @param event the window resize event
         */
        void onResized(const sf::Event::SizeEvent & event) override;
    };
}

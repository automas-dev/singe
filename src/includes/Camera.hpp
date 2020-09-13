#pragma once

#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>

namespace game {
    class Camera {
        sf::Vector2u screen;
        glm::vec2 rot;
        glm::vec3 pos;

    public:
        Camera(void);
        virtual ~Camera();

        void setScreen(unsigned width, unsigned height);

        void setScreen(sf::Vector2u screen);

        void setRotation(float x, float y);

        void setRotation(sf::Vector2f rot);

        void setRotation(glm::vec2 rot);

        /**
         * Rotate on the x an y axis in degrees.
         */
        void rotate(float x, float y);

        void setPosition(float x, float y, float z);

        void setPosition(sf::Vector3f pos);

        void setPosition(glm::vec3 pos);

        void move(float x, float y, float z);

        void moveLook(float x, float y, float z);

        void pushTransform(void);

        void popTransform(void);
    };
}

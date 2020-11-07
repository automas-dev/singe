#pragma once

#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <memory>

namespace Tom::s3e {

    class Camera {
    public:
        enum ProjectionMode {
            Perspective,
            Orthographic
        };

    private:
        sf::Vector2u screen;
        glm::vec2 rot;
        glm::vec3 pos;

        ProjectionMode projMode = Perspective;

        float fov = 45.0f;

    public:
        typedef std::shared_ptr<Camera> Ptr;
        typedef std::shared_ptr<const Camera> ConstPtr;

        Camera(void);
        virtual ~Camera();

        // TODO: update()

        float getFov(void);

        void setFov(float fov);

        void setScreenSize(unsigned width, unsigned height);

        void setScreenSize(sf::Vector2u screen);

        ProjectionMode getProjection(void);

        void setProjection(ProjectionMode mode);

        const glm::vec2 &getRotation() const;

        void setRotation(float x, float y);

        void setRotation(sf::Vector2f rot);

        void setRotation(glm::vec2 rot);

        /**
         * Rotate on the x an y axis in degrees.
         */
        void rotate(float x, float y);

        const glm::vec3 &getPosition() const;

        void setPosition(float x, float y, float z);

        void setPosition(sf::Vector3f pos);

        void setPosition(glm::vec3 pos);

        void move(float x, float y, float z);

        void moveLook(float x, float y, float z);

        void pushTransform(void);

        void popTransform(void);

        glm::mat4 projMatrix(void);

        glm::mat4 viewMatrix(void);

        static Ptr create(void);
    };
}

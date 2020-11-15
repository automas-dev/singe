#pragma once

#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <memory>

namespace Tom::s3e {

    /**
     * First person camera used to generate the projection and view matrices.
     */
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
        /**
         * A shared pointer that manages a Camera.
         */
        typedef std::shared_ptr<Camera> Ptr;

        /**
         * A constant shared pointer that manages a Camera.
         */
        typedef std::shared_ptr<const Camera> ConstPtr;

        /**
         * Construct a new Camera with position (0, 0, 0) and rotation (0, 0)
         */
        Camera(void);

        /**
         * Destruct the camera.
         */
        virtual ~Camera();

        /**
         * Get the current Field of View (FOV).
         *
         * @return the current FOV
         */
        float getFov(void);

        /**
         * Set the Field of View (FOV).
         *
         * @param fov the field of view in degrees of view
         */
        void setFov(float fov);

        /**
         * Tell the camera what the current screen size in pixels.
         * This is used when generating the projection matrix.
         *
         * @param screen the screen size in pixels
         */
        void setScreenSize(sf::Vector2u screen);

        /**
         * Get the current projection mode. Orthographic or Perspective.
         * The projection mode is used to generate the projection matrix.
         *
         * @returns the current perspective mode
         */
        ProjectionMode getProjection(void);

        /**
         * Set the projection mode. Orthographic or Perspective.
         * This is used to generate the projection matrix.
         *
         * @param mode the new projection mode
         */
        void setProjection(ProjectionMode mode);

        /**
         * Get the current rotation in degrees.
         *
         * @returns the current rotation
         */
        const glm::vec2 &getRotation() const;

        /**
         * Set the rotation in degrees.
         *
         * @param rot the new rotation
         */
        void setRotation(glm::vec2 rot);

        /**
         * Rotate on the x an y axis in degrees.
         * `delta` is added to the current rotation.
         *
         * @param delta the delta rotation
         */
        void rotate(glm::vec2 delta);

        /**
         * Get the current position.
         *
         * @return the current position
         */
        const glm::vec3 &getPosition() const;

        /**
         * Set the position.
         *
         * @param pos the new position
         */
        void setPosition(glm::vec3 pos);

        /**
         * Move the camera. `delta` is added to the current position.
         *
         * @param delta the delta position
         */
        void move(glm::vec3 delta);

        /**
         * Move the camera using the current rotation as the forward vector.
         *
         * @param delta the delta position
         */
        void moveDolly(glm::vec3 delta);

        /**
         * Get the projection matrix. This is generated from screen size and
         * the current field of view.
         *
         * @return the projection matrix
         */
        glm::mat4 projMatrix(void);

        /**
         * Get the view matrix. This is generated from the current position and
         * rotation.
         *
         * @return the view matrix
         */
        glm::mat4 viewMatrix(void);

        /**
         * Create a new Camera that is managed by as std::shared_ptr.
         *
         * @return a shared pointer to a new Camera
         */
        static Ptr create(void);
    };
}

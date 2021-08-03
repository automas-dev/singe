#pragma once

#include <GL/glew.h>

#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <memory>

#include "s3e/Support/Transform3d.hpp"

namespace Tom::s3e {

    /**
     * First person camera used to generate the projection and view matrices.
     */
    class Camera : public Transform3d {
    public:
        /**
         * Projection mode for projectino matrix.
         */
        enum ProjectionMode {
            /// Perspective projection using fov
            Perspective,
            /// Orthographic projection
            Orthographic,
        };

    private:
        sf::Vector2u screenSize;
        ProjectionMode projectionMode;
        float fov;
        float pitch;
        float yaw;

    public:
        /**
         * A shared pointer that manages a Camera.
         */
        using Ptr = std::shared_ptr<Camera>;

        /**
         * A shared pointer that manages a const Camera.
         */
        using ConstPtr = std::shared_ptr<const Camera>;

        /**
         * Construct a new Camera with position (0, 0, 0) and rotation (0, 0)
         */
        Camera();

        /**
         * Destruct the camera.
         */
        virtual ~Camera();

        /**
         * Get the current Field of View (FOV).
         *
         * @return the current FOV
         */
        float getFov();

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
        void setScreenSize(const sf::Vector2u & screen);

        /**
         * Get the current projection mode. Orthographic or Perspective.
         * The projection mode is used to generate the projection matrix.
         *
         * @returns the current perspective mode
         */
        ProjectionMode getProjection();

        /**
         * Set the projection mode. Orthographic or Perspective.
         * This is used to generate the projection matrix.
         *
         * @param mode the new projection mode
         */
        void setProjection(ProjectionMode mode);

        /**
         * Rotate by the euler delta.
         *
         * @param delta the delta rotation
         */
        void rotateEuler(const glm::vec3 & delta);

        /**
         * Move the camera using the current rotation as the forward vector.
         *
         * @param delta the delta position
         */
        void moveDolly(const glm::vec3 & delta);

        /**
         * Get the view matrix
         *
         * @return the view matrix
         */
        glm::mat4 toMatrix() const;

        /**
         * Get the projection matrix. This is generated from screen size and
         * the current field of view.
         *
         * @return the projection matrix
         */
        glm::mat4 projMatrix(void);
    };
}

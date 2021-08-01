#pragma once

#include <glm/glm.hpp>

namespace Tom::s3e {
    /**
     * A Generic 3d transform that can be used to calculate a 4x4 transform
     * matrix.
     */
    class Transform3d {
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;

    public:
        /**
         * Default constructor with position = 0, rotation = 0, scale = 1
         */
        Transform3d();

        /**
         * Destructor
         */
        virtual ~Transform3d();

        /**
         * Default copy constructor.
         */
        Transform3d(const Transform3d & other) = default;

        /**
         * Default move constructor.
         */
        Transform3d(Transform3d && other) = default;

        /**
         * Default copy assignment operator.
         */
        Transform3d & operator=(const Transform3d & other) = default;

        /**
         * Default move assignment operator.
         */
        Transform3d & operator=(Transform3d && other) = default;

        /**
         * Move by adding delta to the position.
         *
         * @param delta the delta position
         */
        void move(const glm::vec3 & delta);

        /**
         * Rotate by adding delta to the rotation.
         *
         * @param delta the delta rotation
         */
        void rotate(const glm::vec3 & delta);

        /**
         * Scale by multiplying the scale by scale.
         *
         * @param scale the delta scale
         */
        void scale(const glm::vec3 & scale);

        /**
         * Get the current position.
         *
         * @return the current position
         */
        const glm::vec3 & getPosition() const;

        /**
         * Set the position.
         *
         * @param position the new position
         */
        void setPosition(const glm::vec3 & position);

        /**
         * Get the current rotation.
         *
         * @return the current rotation
         */
        const glm::vec3 & getRotation() const;

        /**
         * Set the rotation.
         *
         * @param rotation the new rotation
         */
        void setRotation(const glm::vec3 & rotation);

        /**
         * Get the current scale.
         *
         * @return the current scale
         */
        const glm::vec3 & getScale() const;

        /**
         * Set the scale.
         *
         * @param scale the new scale
         */
        void setScale(const glm::vec3 & scale);

        /**
         * Get the transform matrix. This is generated from position, rotation
         * and scale.
         *
         * @return the transform matrix
         */
        glm::mat4 toMatrix() const;
    };
};
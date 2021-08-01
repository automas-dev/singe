#include "s3e/Core/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "s3e/Support/Util.hpp"
#include "s3e/Support/log.hpp"

namespace Tom::s3e {

    Camera::Camera() : projectionMode(Perspective), fov(45.0f) {}

    Camera::~Camera() {}

    void Camera::setFov(float fov) {
        this->fov = fov;
    }

    float Camera::getFov() {
        return fov;
    }

    void Camera::setScreenSize(sf::Vector2u screen) {
        this->screenSize = screen;
    }

    Camera::ProjectionMode Camera::getProjection() {
        return projectionMode;
    }

    void Camera::setProjection(Camera::ProjectionMode mode) {
        projectionMode = mode;
    }

    void Camera::moveDolly(glm::vec3 delta) {
        float yRot = glm::radians(getRotation().y);

        float dz = -delta.x * std::sin(yRot) + delta.z * std::cos(yRot);
        float dx = -delta.x * -std::cos(yRot) + delta.z * std::sin(yRot);

        float dy = delta.y;

        move(glm::vec3(dx, dy, dz));
    }

    glm::mat4 Camera::toMatrix() const {
        glm::mat4 m(1);
        m = glm::rotate(m, glm::radians(getRotation().x), glm::vec3(1, 0, 0));
        m = glm::rotate(m, glm::radians(getRotation().y), glm::vec3(0, 1, 0));
        m = glm::translate(m, -getPosition());
        return m;
    }

    glm::mat4 Camera::projMatrix() {
        float r = (float)screenSize.x / (float)screenSize.y;
        if (projectionMode == Perspective)
            return glm::perspective(glm::radians(fov), r, 0.1f, 100.0f);
        else
            return glm::ortho<float>(0, screenSize.x, screenSize.y, 0);
    }
}

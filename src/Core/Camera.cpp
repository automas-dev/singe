#include "singe/Core/Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "singe/Support/Util.hpp"
#include "singe/Support/log.hpp"

namespace singe {

    constexpr float rotXMin = glm::radians(-89.f);
    constexpr float rotXMax = glm::radians(89.f);

    Camera::Camera() : projectionMode(Perspective), fov(45.0f) {}

    Camera::~Camera() {}

    void Camera::setFov(float fov) {
        this->fov = fov;
    }

    float Camera::getFov() {
        return fov;
    }

    void Camera::setScreenSize(const sf::Vector2u & screen) {
        this->screenSize = screen;
    }

    Camera::ProjectionMode Camera::getProjection() {
        return projectionMode;
    }

    void Camera::setProjection(Camera::ProjectionMode mode) {
        projectionMode = mode;
    }

    void Camera::rotateEuler(const glm::vec3 & delta) {
        pitch += delta.x;
        yaw += delta.y;
        pitch = glm::clamp(pitch, rotXMin, rotXMax);

        glm::quat qPitch = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
        glm::quat qYaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));
        glm::quat rotation = glm::normalize(qPitch * qYaw);
        setRotation(rotation);
    }

    void Camera::moveDolly(const glm::vec3 & delta) {
        float yRot = yaw;

        float dz = delta.x * std::sin(yRot) + delta.z * std::cos(yRot);
        float dx = delta.x * std::cos(yRot) - delta.z * std::sin(yRot);

        float dy = delta.y;

        move(glm::vec3(dx, dy, dz));
    }

    glm::mat4 Camera::toMatrix() const {
        return glm::translate(glm::toMat4(getRotation()), -getPosition());
    }

    glm::mat4 Camera::projMatrix() {
        float r = (float)screenSize.x / (float)screenSize.y;
        if (projectionMode == Perspective)
            return glm::perspective(glm::radians(fov), r, 0.1f, 100.0f);
        else
            return glm::ortho<float>(0, screenSize.x, screenSize.y, 0);
    }
}

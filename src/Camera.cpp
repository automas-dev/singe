#include "s3e/Camera.hpp"
#include "s3e/Util.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Tom::s3e {

    Camera::Camera(): rot(0, 0), pos(0, 0, 0) { }

    Camera::~Camera() { }

    void Camera::setFov(float fov) {
        this->fov = fov;
    }

    float Camera::getFov() {
        return fov;
    }

    void Camera::setScreenSize(sf::Vector2u screen) {
        this->screen = screen;
    }

    const glm::vec2 & Camera::getRotation() const {
        return rot;
    }

    void Camera::setRotation(glm::vec2 rot) {
        this->rot = rot;
    }

    void Camera::rotate(glm::vec2 delta) {
        rot -= delta;
        rot.x = glm::clamp(rot.x, -89.0f, 89.0f);
    }

    const glm::vec3 & Camera::getPosition() const {
        return pos;
    }

    void Camera::setPosition(glm::vec3 pos) {
        this->pos = pos;
    }

    void Camera::move(glm::vec3 delta) {
        pos += delta;
    }

    void Camera::moveDolly(glm::vec3 delta) {
        float yRot = glm::radians(rot.y);

        float dz = -delta.x * std::sin(yRot) + delta.z * std::cos(yRot);
        float dx = -delta.x * -std::cos(yRot) + delta.z * std::sin(yRot);

        float dy = delta.y;

        pos += glm::vec3(dx, dy, dz);
    }

    glm::mat4 Camera::projMatrix() {
        float r = (float)screen.x / (float)screen.y;
        return glm::perspective(glm::radians(fov), r, 0.1f, 100.0f);
    }

    glm::mat4 Camera::viewMatrix() {
        glm::mat4 m (1);
        m = glm::rotate(m, -glm::radians(rot.x), glm::vec3(1, 0, 0));
        m = glm::rotate(m, -glm::radians(rot.y), glm::vec3(0, 1, 0));
        m = glm::translate(m, -pos);
        return m;
    }

    Camera::Ptr Camera::create() {
        auto camera = std::make_shared<Camera>();
        return camera;
    }
}

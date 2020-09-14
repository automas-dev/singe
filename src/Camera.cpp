#include "Camera.hpp"

namespace game {
    
    Camera::Camera(): rot(0, 0), pos(0, 0, 0) { }

    Camera::~Camera() { }

    void Camera::setScreen(unsigned width, unsigned height) {
        setScreen({width, height});
    }

    void Camera::setScreen(sf::Vector2u screen) {
        this->screen = screen;
    }

    void Camera::setRotation(float x, float y) {
        rot = {x, y};
    }

    void Camera::setRotation(sf::Vector2f rot) {
        this->rot = {rot.x, rot.y};
    }

    void Camera::setRotation(glm::vec2 rot) {
        this->rot = rot;
    }

    /**
     * Rotate on the x an y axis in degrees.
     */
    void Camera::rotate(float x, float y) {
        rot -= glm::vec2(x, y);
        rot.x = glm::clamp(rot.x, -89.0f, 89.0f);
    }

    void Camera::setPosition(float x, float y, float z) {
        pos = {x, y, z};
    }

    void Camera::setPosition(sf::Vector3f pos) {
        this->pos = {pos.x, pos.y, pos.z};
    }

    void Camera::setPosition(glm::vec3 pos) {
        this->pos = pos;
    }

    void Camera::move(float x, float y, float z) {
        pos += glm::vec3(x, y, z);
    }

    void Camera::moveLook(float x, float y, float z) {
        float yRot = glm::radians(rot.y);

        float dz = -x * std::sin(yRot) + z * std::cos(yRot);
        float dx = -x * -std::cos(yRot) + z * std::sin(yRot);

        float dy = y;

        pos += glm::vec3(dx, dy, dz);
    }

    void Camera::pushTransform() {
        glPushMatrix();
        glScalef(1.0f, (float)screen.x / screen.y, 1.0f);
        glRotatef(-this->rot.x, 1, 0, 0);
        glRotatef(-this->rot.y, 0, 1, 0);
        glScalef(1.0f, (float)screen.y / screen.x, 1.0f);
        glTranslatef(-this->pos.x, -this->pos.y, -this->pos.z);
    }

    void Camera::popTransform() {
        glPopMatrix();
    }

    Camera::Ptr Camera::create() {
        auto camera = std::make_shared<Camera>();
        return camera;
    }
}

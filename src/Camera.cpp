#include "Camera.hpp"
#include "Util.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace game {

    Camera::Camera(): rot(0, 0), pos(0, 0, 0) { }

    Camera::~Camera() { }

    void Camera::setFov(float fov) {
        this->fov = fov;
    }

    float Camera::getFov() {
        return fov;
    }

    void Camera::setScreenSize(unsigned width, unsigned height) {
        setScreenSize({width, height});
    }

    void Camera::setScreenSize(sf::Vector2u screen) {
        this->screen = screen;
    }

    const glm::vec2 &Camera::getRotation() const {
        return rot;
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

    const glm::vec3 &Camera::getPosition() const {
        return pos;
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

        // return matFromVecs(pos, glm::radians(glm::vec3(rot.x, 0, rot.y)));

        // auto rx = glm::radians(rot.x);
        // auto ry = glm::radians(rot.y);
        // auto x = glm::sin(ry) * glm::sin(rx);
        // auto y = glm::sin(rx);
        // auto z = glm::sin(ry) * glm::cos(rx);
        // glm::vec3 center(x, y, z);
        // center += pos;
        // return glm::lookAt(pos, center, glm::vec3(0, 1, 0));
    }

    Camera::Ptr Camera::create() {
        auto camera = std::make_shared<Camera>();
        return camera;
    }
}

#include "s3e/Support/Transform3d.hpp"

#include "s3e/Support/Util.hpp"

namespace Tom::s3e {

    Transform3d::Transform3d() : m_position(0), m_rotation(glm::vec3(0, 0, 0)), m_scale(1) {}

    Transform3d::~Transform3d() {}

    void Transform3d::move(const glm::vec3 & delta) {
        m_position += delta;
    }

    void Transform3d::rotateEuler(const glm::vec3 & delta) {
        rotate(glm::quat(delta));
    }

    void Transform3d::rotate(const glm::quat & delta) {
        m_rotation = delta * m_rotation;
    }

    void Transform3d::scale(const glm::vec3 & scale) {
        m_scale *= scale;
    }

    const glm::vec3 & Transform3d::getPosition() const {
        return m_position;
    }

    void Transform3d::setPosition(const glm::vec3 & position) {
        m_position = position;
    }

    glm::vec3 Transform3d::getRotationEuler() const {
        return glm::eulerAngles(m_rotation);
    }

    const glm::quat & Transform3d::getRotation() const {
        return m_rotation;
    }

    void Transform3d::setRotation(const glm::quat & rotation) {
        m_rotation = rotation;
    }

    const glm::vec3 & Transform3d::getScale() const {
        return m_scale;
    }

    void Transform3d::setScale(const glm::vec3 & scale) {
        m_scale = scale;
    }

    glm::mat4 Transform3d::toMatrix() const {
        auto translate = glm::translate(glm::mat4(1), m_position);
        auto rotate = glm::toMat4(m_rotation);
        auto scale = glm::scale(glm::mat4(1), m_scale);
        return translate * rotate * scale;
    }
};
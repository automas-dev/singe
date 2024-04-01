#include "Physics.hpp"

namespace singe {
    PhysicsObject::PhysicsObject() : Transform() {}

    PhysicsObject::PhysicsObject(const vec3 & position,
                                 const quat & rotation,
                                 const vec3 & scale)
        : PhysicsObject(position, rotation, scale, vec3(0), quat(), 0.0, 0.0) {}

    PhysicsObject::PhysicsObject(const vec3 & position,
                                 const quat & rotation,
                                 const vec3 & scale,
                                 const vec3 & velocity,
                                 const quat & spin,
                                 const float friction,
                                 const float twist)
        : Transform(position, rotation, scale),
          velocity(velocity),
          spin(spin),
          friction(friction),
          twist(twist) {}

    void PhysicsObject::setCollider(const Collider::Ptr & collider) {
        this->collider = collider;
    }

    const Collider::Ptr & PhysicsObject::getCollider() const {
        return collider;
    }
}

namespace singe {
    PhysicsSim::PhysicsSim() {}

    void PhysicsSim::addObject(const PhysicsObject::Ptr & object) {
        objects.push_back(object);
    }

    void PhysicsSim::addCollider(const Collider::Ptr & collider) {
        colliders.push_back(collider);
    }

    void PhysicsSim::step(float dt, int subStep) {
        float subDt = dt;
        if (subStep > 0)
            subDt /= subStep;

        for (int i = 0; i < subStep; i++) {
            // Move all objects
            for (auto & obj : objects) {
                if (obj->sleep)
                    continue;

                // obj->velocity *= obj->friction * subDt;
                obj->velocity.y += gravity;
                obj->move(obj->velocity * subDt);

                // obj->spin *= obj->twist * subDt;
                obj->rotate(obj->spin * subDt);
            }

            // Check for collision
            for (auto & obj : objects) {
                for (auto & col : colliders) {
                    if (col->testCollision(*obj->getCollider()))
                        obj->velocity = glm::vec3(0);
                }
            }
        }
    }
}

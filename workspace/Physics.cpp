#include "Physics.hpp"

namespace singe {
    PhysicsSim::PhysicsSim() {}

    void PhysicsSim::addObject(const PhysicsObject::Ptr & object) {
        objects.push_back(object);
    }

    void PhysicsSim::step(float dt, int subStep) {
        for (int i = 0; i < subStep; i++) {
            // ...
        }
    }
}

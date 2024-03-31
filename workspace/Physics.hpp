#pragma once

#include <memory>
#include <vector>

#include "Collision.hpp"

namespace singe {
    using std::shared_ptr;
    using std::vector;

    class PhysicsObject {
    public:
        using Ptr = shared_ptr<PhysicsObject>;
        using ConstPtr = const shared_ptr<PhysicsObject>;

    private:
    public:
    };

    class PhysicsSim {
    public:
        using Ptr = shared_ptr<PhysicsSim>;
        using ConstPtr = const shared_ptr<PhysicsSim>;

    private:
        vector<PhysicsObject::Ptr> objects;

    public:
        PhysicsSim();

        void addObject(const PhysicsObject::Ptr & object);

        void step(float dt, int subStep = 10);
    };
}

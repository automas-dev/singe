#pragma once

#include <glm/glm.hpp>
#include <glpp/extra/Transform.hpp>
#include <memory>
#include <vector>

#include "Collision.hpp"

namespace singe {
    using std::shared_ptr;
    using std::vector;
    using glm::vec3;
    using glm::quat;
    using glpp::extra::Transform;

    class Collider {
    public:
        using Ptr = shared_ptr<Collider>;
        using ConstPtr = const shared_ptr<Collider>;

        enum Type {
            ABOX,
            BALL,
        };

    protected:
        Type type;
        union {
            AABB abox;
            Sphere ball;
        };

    public:
        Collider(Type type) : type(type) {}

        Collider(AABB & aabb) : type(ABOX), abox(aabb) {}

        Collider(Sphere & sphere) : type(BALL), ball(sphere) {}

        bool testCollision(const Collider & other) const {
            if (type == ABOX) {
                if (other.type == ABOX) {
                    return collides(abox, other.abox);
                }
                else {
                    return collides(other.ball, abox);
                }
            }
            else {
                if (other.type == ABOX) {
                    return collides(ball, other.abox);
                }
                else {
                    return collides(ball, other.ball);
                }
            }
        }
    };

    class PhysicsSim;

    struct PhysicsObject : public Transform {
        using Ptr = shared_ptr<PhysicsObject>;
        using ConstPtr = const shared_ptr<PhysicsObject>;

        vec3 velocity;
        quat spin;
        bool sleep;
        float friction;
        float twist;
        Collider::Ptr collider;

        PhysicsObject();

        PhysicsObject(const vec3 & position,
                      const quat & rotation,
                      const vec3 & scale);

        PhysicsObject(const vec3 & position,
                      const quat & rotation,
                      const vec3 & scale,
                      const vec3 & velocity,
                      const quat & spin,
                      const float friction,
                      const float twist);

        void setCollider(const Collider::Ptr & collider);

        const Collider::Ptr & getCollider() const;

        friend PhysicsSim;
    };

    class PhysicsSim {
    public:
        using Ptr = shared_ptr<PhysicsSim>;
        using ConstPtr = const shared_ptr<PhysicsSim>;

    private:
        vector<PhysicsObject::Ptr> objects;
        vector<Collider::Ptr> colliders;
        float gravity = -9.84;

    public:
        PhysicsSim();

        void addObject(const PhysicsObject::Ptr & object);

        void addCollider(const Collider::Ptr & collider);

        void step(float dt, int subStep = 10);
    };
}

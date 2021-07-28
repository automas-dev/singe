#pragma once

#include <SFML/System.hpp>
#include <atomic>
#include <mutex>
#include <thread>

#include "s3e/Support/log.hpp"
#include "btBulletDynamicsCommon.h"

namespace Tom::s3e {
    class Physics {
        std::thread t;
        mutable std::mutex m;
        std::atomic<bool> running;
        std::atomic<bool> paused;
        sf::Clock clock;
        float updateInterval;

        void worker();

    public:
        btDefaultCollisionConfiguration * collisionConfiguration;
        btCollisionDispatcher * dispatcher;
        btBroadphaseInterface * overlappingPairCache;
        btSequentialImpulseConstraintSolver * solver;
        btDiscreteDynamicsWorld * dynamicsWorld;
        btAlignedObjectArray<btCollisionShape *> collisionShapes;

    public:
        Physics(float updateInterval = 1. / 60);

        virtual ~Physics();

        bool getRunState() const;

        void setRunState(bool run);

        void addRigidBody(btRigidBody * body);

        btRigidBody * makeRigidBody(btCollisionShape * collisionShape,
                                    btScalar mass = 0,
                                    btScalar friction = 0);

        void step(const sf::Time & delta, int maxSubSteps = 10);

        void getTransform(int i, btTransform & trans) const;

        btCollisionObjectArray & getCollisionObjectArray() const;

        void removeObjects();
    };
}

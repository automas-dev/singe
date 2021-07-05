#pragma once

#include <SFML/System.hpp>

#include "btBulletDynamicsCommon.h"
#include "log.hpp"

namespace Tom::s3e {
    class Physics {
    public:
        btDefaultCollisionConfiguration * collisionConfiguration;
        btCollisionDispatcher * dispatcher;
        btBroadphaseInterface * overlappingPairCache;
        btSequentialImpulseConstraintSolver * solver;
        btDiscreteDynamicsWorld * dynamicsWorld;
        btAlignedObjectArray<btCollisionShape *> collisionShapes;

    public:
        Physics(void);

        virtual ~Physics();

        void doThing();

        void addRigidBody(btRigidBody * body);

        btRigidBody * makeRigidBody(btCollisionShape * collisionShape,
                                    btScalar mass = 0,
                                    btScalar friction = 0);

        void loadObjects();

        void update(const sf::Time & delta, int maxSubSteps = 10);

        void getTransform(int i, btTransform & trans);

        btDiscreteDynamicsWorld * getWorld();

        btCollisionObjectArray & getCollisionObjectArray();

        void printObjectsLocations();

        void removeObjects();
    };
}

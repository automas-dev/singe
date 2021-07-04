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
        Physics(void) {
            collisionConfiguration = new btDefaultCollisionConfiguration();
            dispatcher = new btCollisionDispatcher(collisionConfiguration);
            overlappingPairCache = new btDbvtBroadphase();
            solver = new btSequentialImpulseConstraintSolver;
            dynamicsWorld = new btDiscreteDynamicsWorld(
                dispatcher, overlappingPairCache, solver, collisionConfiguration);

            dynamicsWorld->setGravity(btVector3(0, -10, 0));
        }

        void doThing() {
            for (int i = 0; i < 150; i++) {
                update(sf::seconds(1.f / 60.f));
                printObjectsLocations();
            }
        }

        void loadObjects() {
            {
                btCollisionShape * groundShape = new btBoxShape(
                    btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

                collisionShapes.push_back(groundShape);

                btTransform groundTransform;
                groundTransform.setIdentity();
                groundTransform.setOrigin(btVector3(0, -56, 0));
                groundTransform.setRotation(
                    btQuaternion(btVector3(1, 0, 0), 3.14 * 0.03));

                btScalar mass(0.);

                // rigidbody is dynamic if and only if mass is non zero,
                // otherwise static
                bool isDynamic = (mass != 0.f);

                btVector3 localInertia(0, 0, 0);
                if (isDynamic)
                    groundShape->calculateLocalInertia(mass, localInertia);

                // using motionstate is optional, it provides interpolation
                // capabilities, and only synchronizes 'active' objects
                btDefaultMotionState * myMotionState =
                    new btDefaultMotionState(groundTransform);
                btRigidBody::btRigidBodyConstructionInfo rbInfo(
                    mass, myMotionState, groundShape, localInertia);
                rbInfo.m_friction = 1.f;
                btRigidBody * body = new btRigidBody(rbInfo);

                // add the body to the dynamics world
                dynamicsWorld->addRigidBody(body);
            }

            {
                // create a dynamic rigidbody

                // btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
                btCollisionShape * colShape = new btSphereShape(btScalar(1.));
                collisionShapes.push_back(colShape);

                /// Create Dynamic Objects
                btTransform startTransform;
                startTransform.setIdentity();

                btScalar mass(1.f);

                // rigidbody is dynamic if and only if mass is non zero,
                // otherwise static
                bool isDynamic = (mass != 0.f);

                btVector3 localInertia(0, 0, 0);
                if (isDynamic)
                    colShape->calculateLocalInertia(mass, localInertia);

                startTransform.setOrigin(btVector3(2, 10, 0));

                // using motionstate is recommended, it provides interpolation
                // capabilities, and only synchronizes 'active' objects
                btDefaultMotionState * myMotionState =
                    new btDefaultMotionState(startTransform);
                btRigidBody::btRigidBodyConstructionInfo rbInfo(
                    mass, myMotionState, colShape, localInertia);
                rbInfo.m_friction = 1.f;
                btRigidBody * body = new btRigidBody(rbInfo);
                body->setLinearVelocity(btVector3(0, 0, 0));

                dynamicsWorld->addRigidBody(body);
            }
        }

        void update(const sf::Time & delta, int maxSubSteps = 10) {
            dynamicsWorld->stepSimulation(delta.asSeconds(), maxSubSteps);
        }

        void getTransform(int i, btTransform & trans) {
            btCollisionObject * obj = dynamicsWorld->getCollisionObjectArray()[i];
            btRigidBody * body = btRigidBody::upcast(obj);
            if (body && body->getMotionState()) {
                body->getMotionState()->getWorldTransform(trans);
            }
            else {
                trans = obj->getWorldTransform();
            }
        }

        void printObjectsLocations() {
            for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--) {
                btTransform trans;
                getTransform(j, trans);
                SPDLOG_INFO("world pos object {} = {},{},{}", j,
                            float(trans.getOrigin().getX()),
                            float(trans.getOrigin().getY()),
                            float(trans.getOrigin().getZ()));
            }
        }

        void removeObjects() {
            for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
                btCollisionObject * obj =
                    dynamicsWorld->getCollisionObjectArray()[i];
                btRigidBody * body = btRigidBody::upcast(obj);
                if (body && body->getMotionState()) {
                    delete body->getMotionState();
                }
                dynamicsWorld->removeCollisionObject(obj);
                delete obj;
            }

            for (int j = 0; j < collisionShapes.size(); j++) {
                btCollisionShape * shape = collisionShapes[j];
                collisionShapes[j] = 0;
                delete shape;
            }
        }

        virtual ~Physics() {
            removeObjects();

            delete dynamicsWorld;
            delete solver;
            delete overlappingPairCache;
            delete dispatcher;
            delete collisionConfiguration;
        }
    };
}

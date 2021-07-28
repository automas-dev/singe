#include "s3e/Core/Physics.hpp"

#include <chrono>

namespace Tom::s3e {
    Physics::Physics(float updateInterval) : updateInterval(updateInterval) {
        collisionConfiguration = new btDefaultCollisionConfiguration();
        dispatcher = new btCollisionDispatcher(collisionConfiguration);
        overlappingPairCache = new btDbvtBroadphase();
        solver = new btSequentialImpulseConstraintSolver;
        dynamicsWorld = new btDiscreteDynamicsWorld(
            dispatcher, overlappingPairCache, solver, collisionConfiguration);

        dynamicsWorld->setGravity(btVector3(0, -10, 0));

        running = true;
        paused = true;
        t = std::thread(&Physics::worker, this);
    }

    Physics::~Physics() {
        running = false;
        removeObjects();

        t.join();

        delete dynamicsWorld;
        delete solver;
        delete overlappingPairCache;
        delete dispatcher;
        delete collisionConfiguration;
    }

    void Physics::worker() {
        SPDLOG_DEBUG("Physics worker starting");
        clock.restart();
        while (running) {
            auto delta = clock.restart();

            if (!paused) {
                step(delta);
            }

            auto soFar = clock.getElapsedTime();

            float sleep = updateInterval - soFar.asSeconds();
            if (sleep > 0) {
                std::chrono::duration<float> duration(sleep);
                std::this_thread::sleep_for(duration);
            }
        }
        SPDLOG_DEBUG("Physics worker ending");
    }

    bool Physics::getRunState() const {
        return !paused;
    }

    void Physics::setRunState(bool run) {
        paused = !run;
    }

    void Physics::addRigidBody(btRigidBody * body) {
        collisionShapes.push_back(body->getCollisionShape());
        dynamicsWorld->addRigidBody(body);
    }

    btRigidBody * Physics::makeRigidBody(btCollisionShape * collisionShape,
                                         btScalar mass,
                                         btScalar friction) {

        btTransform groundTransform;
        groundTransform.setIdentity();

        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            collisionShape->calculateLocalInertia(mass, localInertia);

        // using motionstate is optional, it provides interpolation
        // capabilities, and only synchronizes 'active' objects
        btDefaultMotionState * myMotionState =
            new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,
                                                        myMotionState,
                                                        collisionShape,
                                                        localInertia);
        rbInfo.m_friction = friction;
        btRigidBody * body = new btRigidBody(rbInfo);

        // add the body to the dynamics world
        addRigidBody(body);

        return body;
    }

    void Physics::step(const sf::Time & delta, int maxSubSteps) {
        std::scoped_lock lk(m);
        dynamicsWorld->stepSimulation(delta.asSeconds(), maxSubSteps);
    }

    void Physics::getTransform(int i, btTransform & trans) const {
        std::scoped_lock lk(m);
        btCollisionObject * obj = dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody * body = btRigidBody::upcast(obj);
        if (body && body->getMotionState()) {
            body->getMotionState()->getWorldTransform(trans);
        }
        else {
            trans = obj->getWorldTransform();
        }
    }

    btCollisionObjectArray & Physics::getCollisionObjectArray() const {
        std::scoped_lock lk(m);
        return dynamicsWorld->getCollisionObjectArray();
    }

    void Physics::removeObjects() {
        std::scoped_lock lk(m);
        for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
            btCollisionObject * obj = dynamicsWorld->getCollisionObjectArray()[i];
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
}

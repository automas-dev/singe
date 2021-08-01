#include "s3e/Core/Physics.hpp"

#include <chrono>

namespace Tom::s3e {
    Physics::Physics()
        : collisionConfiguration(new btDefaultCollisionConfiguration()),
          dispatcher(new btCollisionDispatcher(collisionConfiguration)),
          overlappingPairCache(new btDbvtBroadphase()),
          solver(new btSequentialImpulseConstraintSolver()),
          dynamicsWorld(new btDiscreteDynamicsWorld(
              dispatcher, overlappingPairCache, solver, collisionConfiguration)) {
        dynamicsWorld->setGravity(btVector3(0, -10, 0));
    }

    Physics::~Physics() {
        removeObjects();

        delete dynamicsWorld;
        delete solver;
        delete overlappingPairCache;
        delete dispatcher;
        delete collisionConfiguration;
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
        dynamicsWorld->stepSimulation(delta.asSeconds(), maxSubSteps);
    }

    void Physics::getTransform(int i, btTransform & trans) const {
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
        return dynamicsWorld->getCollisionObjectArray();
    }

    void Physics::removeObjects() {
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

namespace Tom::s3e {

    ThreadedPhysics::ThreadedPhysics(float updateInterval)
        : Physics(),
          updateInterval(updateInterval),
          running(true),
          paused(true),
          lk(m, std::defer_lock) {
        t = std::thread(&ThreadedPhysics::worker, this);
    }

    ThreadedPhysics::~ThreadedPhysics() {}

    void ThreadedPhysics::stop() {
        Logging::Core->debug("ThreadedPhysics stop");
        running = false;
    }

    void ThreadedPhysics::join() {
        Logging::Core->debug("ThreadedPhysics join");
        t.join();
    }

    void ThreadedPhysics::worker() {
        Logging::Core->debug("ThreadedPhysics worker starting {}", running);
        clock.restart();
        while (running) {
            std::unique_lock lk(m);
            auto delta = clock.restart();

            if (!paused) {
                step(delta);
            }

            auto soFar = clock.getElapsedTime();
            lk.unlock();

            float sleep = updateInterval - soFar.asSeconds();
            if (sleep > 0) {
                std::chrono::duration<float> duration(sleep);
                std::this_thread::sleep_for(duration);
            }
        }
        Logging::Core->debug("ThreadedPhysics worker ending");
    }

    bool ThreadedPhysics::getRunState() const {
        return !paused;
    }

    void ThreadedPhysics::setRunState(bool run) {
        paused = !run;
        Logging::Core->debug("ThreadedPhysics run state set to {}", run);
    }

    void ThreadedPhysics::lock() {
        lk.lock();
    }

    void ThreadedPhysics::unlock() {
        lk.unlock();
    }
};

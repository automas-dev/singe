#pragma once

#include <SFML/System.hpp>
#include <atomic>
#include <mutex>
#include <thread>

#include "btBulletDynamicsCommon.h"
#include "s3e/Support/log.hpp"

namespace Tom::s3e {

    /**
     * Simulate physics and perform collision detection.
     */
    class Physics {
    public:
        btDefaultCollisionConfiguration * collisionConfiguration;
        btCollisionDispatcher * dispatcher;
        btBroadphaseInterface * overlappingPairCache;
        btSequentialImpulseConstraintSolver * solver;
        btDiscreteDynamicsWorld * dynamicsWorld;
        btAlignedObjectArray<btCollisionShape *> collisionShapes;

    public:
        /**
         * Create a new Physics object and specify the update interval.
         */
        Physics();

        /**
         * Stop the thread and release resources from the world.
         */
        virtual ~Physics();

        /**
         * Add a rigid body to the world.
         *
         * @param body the body to add to the world
         */
        void addRigidBody(btRigidBody * body);

        /**
         * Create a new rigid body and add it to the world.
         *
         * The object will by static if the mass is set to 0.
         *
         * @param collisionShape the collision shape
         * @param mass the object mass or 0 for static
         * @param friction the object friction
         *
         * @return the created rigid body
         */
        btRigidBody * makeRigidBody(btCollisionShape * collisionShape,
                                    btScalar mass = 0,
                                    btScalar friction = 0);

        /**
         * Step the simulation with delta time.
         *
         * If maxSubSteps is > 0 interpolation will be performed between steps.
         *
         * @param delta the step delta
         * @param maxSubSteps the number of interpolations between steps
         */
        void step(const sf::Time & delta, int maxSubSteps = 10);

        /**
         * Copy a transform for object i to trans.
         *
         * @param i the object index
         * @param trans the btTransform to populate
         */
        void getTransform(int i, btTransform & trans) const;

        /**
         * Get the collision object array as returned by dynamicsWorld.
         *
         * @return the collision object array
         */
        btCollisionObjectArray & getCollisionObjectArray() const;

        /**
         * Remove all objects from the world.
         */
        void removeObjects();
    };

    /**
     * Physics subclass that performs updates on another thread.
     *
     * Note: this class inits in the paused state. To start processing, call
     * setRunState(bool).
     */
    class ThreadedPhysics : public Physics {
        std::thread t;
        mutable std::mutex m;
        std::unique_lock<std::mutex> lk;
        std::atomic<bool> running;
        std::atomic<bool> paused;
        sf::Clock clock;
        float updateInterval;

        /**
         * The thread worker.
         */
        void worker();

    public:
        /**
         * Create a new Physics object and specify the update interval.
         *
         * @param updateInterval the time in seconds between each update.
         */
        ThreadedPhysics(float updateInterval = 1. / 60);

        virtual ~ThreadedPhysics();

        void stop();

        /**
         * Check if the Physics are being paused or running.
         *
         * @return are updates being processed
         */
        bool getRunState() const;

        /**
         * Set the run state, true is running and false is paused.
         *
         * @param run the run state
         */
        void setRunState(bool run);

        void lock();

        void unlock();
    };
}

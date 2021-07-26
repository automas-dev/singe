#include "s3e/Physics.hpp"

namespace Tom::s3e {
    Physics::Physics() {
        collisionConfiguration = new btDefaultCollisionConfiguration();
        dispatcher = new btCollisionDispatcher(collisionConfiguration);
        overlappingPairCache = new btDbvtBroadphase();
        solver = new btSequentialImpulseConstraintSolver;
        dynamicsWorld = new btDiscreteDynamicsWorld(
            dispatcher, overlappingPairCache, solver, collisionConfiguration);

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

    void Physics::doThing() {
        for (int i = 0; i < 150; i++) {
            update(sf::seconds(1.f / 60.f));
            printObjectsLocations();
        }
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

    void Physics::loadObjects() {
        {
            btCollisionShape * groundShape = new btBoxShape(
                btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

            btRigidBody * body = makeRigidBody(groundShape, 0, 1);

            btTransform groundTransform;
            groundTransform.setIdentity();
            groundTransform.setOrigin(btVector3(0, -56, 0));
            groundTransform.setRotation(
                btQuaternion(btVector3(1, 0, 0), 3.14 * 0.03));
            body->getMotionState()->setWorldTransform(groundTransform);
        }

        {
            btCollisionShape * colShape = new btSphereShape(btScalar(1.));

            btRigidBody * body = makeRigidBody(colShape, 1, 1);

            /// Create Dynamic Objects
            btTransform startTransform;
            startTransform.setIdentity();
            startTransform.setOrigin(btVector3(2, 0, 0));
            body->getMotionState()->setWorldTransform(startTransform);
            body->setLinearVelocity(btVector3(0, 0, 0));
        }
    }

    void Physics::update(const sf::Time & delta, int maxSubSteps) {
        dynamicsWorld->stepSimulation(delta.asSeconds(), maxSubSteps);
    }

    void Physics::getTransform(int i, btTransform & trans) {
        btCollisionObject * obj = dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody * body = btRigidBody::upcast(obj);
        if (body && body->getMotionState()) {
            body->getMotionState()->getWorldTransform(trans);
        }
        else {
            trans = obj->getWorldTransform();
        }
    }

    btDiscreteDynamicsWorld * Physics::getWorld() {
        return dynamicsWorld;
    }

    btCollisionObjectArray & Physics::getCollisionObjectArray() {
        return dynamicsWorld->getCollisionObjectArray();
    }

    void Physics::printObjectsLocations() {
        for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--) {
            btTransform trans;
            getTransform(j, trans);
            SPDLOG_INFO("world pos object {} = {},{},{}", j,
                        float(trans.getOrigin().getX()),
                        float(trans.getOrigin().getY()),
                        float(trans.getOrigin().getZ()));
        }
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

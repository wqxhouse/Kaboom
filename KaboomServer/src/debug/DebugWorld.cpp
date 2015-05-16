#include "DebugWorld.h"

#include <btBulletDynamicsCommon.h>
#include <osgbCollision/CollisionShapes.h>

#include <util/ConfigSettings.h>

#include "../debug/OsgBulletDebugViewer.h"

DebugWorld::DebugWorld(ConfigSettings *configSettings)
        : World(configSettings),
          debugViewer(nullptr) {
    std::string debug;
    configSettings->getValue(ConfigSettings::str_server_debug_mode, debug);
    debugMode = debug == "true" ? true : false;

    if (debugMode) {
        debugViewer = new OsgBulletDebugViewer(configSettings);
        debugViewer->init();
        world.setDebugDrawer(debugViewer->getDbgDraw());
    }
}

DebugWorld::~DebugWorld() {
    delete debugViewer;
}

void DebugWorld::stepSimulation(float timeStep, int maxSubSteps) {
    World::stepSimulation(timeStep, maxSubSteps);

    if (debugMode) {
        debugViewer->getDbgDraw()->BeginDraw();
        world.debugDrawWorld();
        debugViewer->getDbgDraw()->EndDraw();
    }
}

void DebugWorld::addRigidBody(btRigidBody *rigidBody) {
    if (debugMode) {
        auto collisionShape = rigidBody->getCollisionShape();

        if (collisionShape != nullptr) {
            osg::ref_ptr<osg::Node> node = osgbCollision::osgNodeFromBtCollisionShape(collisionShape);
            debugViewer->addNode(node);
        }
    }

    World::addRigidBody(rigidBody);
}

void DebugWorld::renderDebugFrame() {
    if (debugMode) {
        debugViewer->renderFrame();
    }
}

void DebugWorld::debugDrawWorld() {
    world.debugDrawWorld();
}

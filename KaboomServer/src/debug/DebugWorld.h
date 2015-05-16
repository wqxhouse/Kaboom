#pragma once

#include "../core/World.h"

class OsgBulletDebugViewer;

class DebugWorld : public World {
public:
    DebugWorld(ConfigSettings *configSettings);
    ~DebugWorld();

    virtual void stepSimulation(float timeStep, int maxSubSteps);

    virtual void addRigidBody(btRigidBody *rigidBody);

    void renderDebugFrame();
    void debugDrawWorld();

private:
    bool debugMode;

    OsgBulletDebugViewer *debugViewer;
};

#pragma once

#include <Windows.h>

#include <osg/NodeCallback>

#include "GameClient.h"

class ClientUpdateCallback : public osg::NodeCallback {
public:
    ClientUpdateCallback(GameClient *client);
    ~ClientUpdateCallback();

    virtual void operator()(osg::Node *node, osg::NodeVisitor *nv);

private:
    GameClient *client;
};

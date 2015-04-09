#pragma once

#include <Windows.h>

#include <osg/NodeCallback>

#include "Client.h"

class ClientUpdateCallback : public osg::NodeCallback {
public:
    ClientUpdateCallback(Client *client);
    ~ClientUpdateCallback();

    virtual void operator()(osg::Node *node, osg::NodeVisitor *nv);

private:
    Client *client;
};

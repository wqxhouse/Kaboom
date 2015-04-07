#pragma once

#include <Windows.h>

#include <osg/NodeCallback>

#include "MockClient.h"

class MockClientUpdateCallback : public osg::NodeCallback {
public:
    MockClientUpdateCallback(MockClient *client);
    ~MockClientUpdateCallback();

    virtual void operator()(osg::Node *node, osg::NodeVisitor *nv);

private:
    MockClient *client;
};

#pragma once

#include <Windows.h>

#include <osg/NodeCallback>

#include "Entity.h"

class PlayerUpdateCallback : public osg::NodeCallback {
public:
    PlayerUpdateCallback(Entity *entity);
    ~PlayerUpdateCallback();

    virtual void operator()(osg::Node *node, osg::NodeVisitor *nv);

private:
    Entity *entity;
};

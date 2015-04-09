#pragma once

#include <osg/NodeCallback>

class PlayerUpdateCallback : public osg::NodeCallback {
public:
    PlayerUpdateCallback();
    ~PlayerUpdateCallback();

    virtual void operator()(osg::Node *node, osg::NodeVisitor *nv);
};

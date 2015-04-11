#pragma once

#include <Windows.h>

#include <osg/Node>

#include "Component.h"

class SceneNodeComponent : public Component {
public:
    SceneNodeComponent(osg::Node *node);
    ~SceneNodeComponent();

    osg::Node *getNode() const;
    void setNode(osg::Node *node);

private:
    osg::Node *node;
};

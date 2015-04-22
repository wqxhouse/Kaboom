#pragma once

#include <core/Component.h>

namespace osg {
    class Node;
}

class SceneNodeComponent : public Component {
public:
    SceneNodeComponent(osg::Node *node);
    ~SceneNodeComponent();

    osg::Node *getNode() const;
    void setNode(osg::Node *node);

private:
    osg::Node *node;
};

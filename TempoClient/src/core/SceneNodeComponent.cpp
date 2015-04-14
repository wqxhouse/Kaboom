#include "SceneNodeComponent.h"

#include <osg/MatrixTransform>

SceneNodeComponent::SceneNodeComponent(osg::Node *node)
    : node(node) {
}

SceneNodeComponent::~SceneNodeComponent() {
}

osg::Node *SceneNodeComponent::getNode() const {
    return node;
}

void SceneNodeComponent::setNode(osg::Node *node) {
    this->node = node;
}

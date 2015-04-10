#include "SceneNodeComponent.h"

SceneNodeComponent::SceneNodeComponent(osg::Node *node)
    : node(node) {
}

SceneNodeComponent::~SceneNodeComponent() {
}

ComponentType SceneNodeComponent::getType() const {
    return SCENE_NODE;
}

osg::Node *SceneNodeComponent::getNode() const {
    return node;
}

void SceneNodeComponent::setNode(osg::Node *node) {
    this->node = node;
}

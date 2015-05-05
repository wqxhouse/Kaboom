#include "SceneNodeComponent.h"

SceneNodeComponent::SceneNodeComponent(osg::ref_ptr<osg::Node> node)
        : node(node) {
}

osg::ref_ptr<osg::Node> SceneNodeComponent::getNode() const {
    return node;
}

void SceneNodeComponent::setNode(osg::ref_ptr<osg::Node> node) {
    this->node = node;
}

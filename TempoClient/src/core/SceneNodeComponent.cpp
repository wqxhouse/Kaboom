#include "SceneNodeComponent.h"

#include <osg/MatrixTransform>

SceneNodeComponent::SceneNodeComponent(osg::Node *node)
    : node(node) {
}

SceneNodeComponent::~SceneNodeComponent() {
}

void SceneNodeComponent::update(const GameStateData &gameState) {
    float x;
    float y;
    float z;

    if (entity->getId() == 0) {
        x = gameState.x1;
        y = gameState.y1;
        z = gameState.z1;
    } else if (entity->getId() == 1) {
        x = gameState.x2;
        y = gameState.y2;
        z = gameState.z2;
    }

    osg::Group *rootNode = static_cast<osg::Group *>(node);
    osg::MatrixTransform *transformationNode = static_cast<osg::MatrixTransform *>(rootNode->getChild(0));

    osg::Matrix mat;
    mat.makeTranslate(osg::Vec3(x, y, z));
    transformationNode->setMatrix(mat);
}

osg::Node *SceneNodeComponent::getNode() const {
    return node;
}

void SceneNodeComponent::setNode(osg::Node *node) {
    this->node = node;
}

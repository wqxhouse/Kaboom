#include "Player.h"

#include <Windows.h>

#include <osg/MatrixTransform>

#include <core/PositionComponent.h>

#include "SceneNodeComponent.h"

Player::Player(unsigned int id)
    : Entity(id) {
}

Player::~Player() {
}

float Player::getX() const {
    return getComponent<PositionComponent>()->getX();
}

float Player::getY() const {
    return getComponent<PositionComponent>()->getY();
}

float Player::getZ() const {
    return getComponent<PositionComponent>()->getZ();
}

void Player::setPosition(float x, float y, float z) {
    getComponent<PositionComponent>()->setX(x);
    getComponent<PositionComponent>()->setY(y);
    getComponent<PositionComponent>()->setZ(z);

    osg::Group *rootNode = static_cast<osg::Group *>(getComponent<SceneNodeComponent>()->getNode());
    osg::MatrixTransform *transformNode = static_cast<osg::MatrixTransform *>(rootNode->getChild(0));

    osg::Matrix transformMat;
    transformMat.makeTranslate(x, y, z);

    transformNode->setMatrix(transformMat);
}

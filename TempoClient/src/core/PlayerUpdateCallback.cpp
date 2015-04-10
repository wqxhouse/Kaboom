#include "PlayerUpdateCallback.h"

#include <osg/MatrixTransform>

#include "../Global.h"
#include "PositionComponent.h"
#include "SceneNodeComponent.h"

PlayerUpdateCallback::PlayerUpdateCallback(Entity *entity)
    : entity(entity) {
}

PlayerUpdateCallback::~PlayerUpdateCallback() {
}

void PlayerUpdateCallback::operator()(osg::Node *node, osg::NodeVisitor *nv) {
    if (g_gameState == NULL) {
        return;
    }

    float x;
    float y;
    float z;

    if (entity->getId() == 1) {
        x = g_gameState->x1;
        y = g_gameState->y1;
        z = g_gameState->z1;
    } else if (entity->getId() == 2) {
        x = g_gameState->x2;
        y = g_gameState->y2;
        z = g_gameState->z2;
    }

    osg::Group *rootNode = static_cast<osg::Group *>(node);
    osg::MatrixTransform *transformationNode = static_cast<osg::MatrixTransform *>(rootNode->getChild(0));

    osg::Matrix mat;
    mat.makeTranslate(osg::Vec3(x, y, z));
    transformationNode->setMatrix(mat);

    PositionComponent *posCom = static_cast<PositionComponent *>(entity->getComponent(POSITION));
    posCom->setX(x);
    posCom->setY(y);
    posCom->setZ(z);

}

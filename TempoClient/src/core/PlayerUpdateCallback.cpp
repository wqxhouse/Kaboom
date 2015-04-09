#include "PlayerUpdateCallback.h"

#include "../Global.h"
#include "Player.h"

PlayerUpdateCallback::PlayerUpdateCallback() {
}

PlayerUpdateCallback::~PlayerUpdateCallback() {
}

void PlayerUpdateCallback::operator()(osg::Node *node, osg::NodeVisitor *nv) {
    Player *player = static_cast<Player *>(node);

    if (player->getEntityData()->id == 1) {
        player->setPosition(g_gameState->x1, g_gameState->y1, g_gameState->z1);
    } else if (player->getEntityData()->id == 2) {
        player->setPosition(g_gameState->x2, g_gameState->y2, g_gameState->z2);
    }
}

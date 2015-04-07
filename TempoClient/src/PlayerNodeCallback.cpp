#include "PlayerNodeCallback.h"

#include <osg/MatrixTransform>

#include "Global.h"
#include "PlayerNode.h"

void PlayerNodeCallback::operator()(osg::Node *node, osg::NodeVisitor *nv) {
    if (player.movingForward) {
        player.velocity.y() = 0.05f;
    } else if (player.movingBackward) {
        player.velocity.y() = -0.05f;
    } else {
        player.velocity.y() = 0;
    }

    if (player.movingLeft) {
        player.velocity.x() = -0.05f;
    } else if (player.movingRight) {
        player.velocity.x() = 0.05f;
    } else {
        player.velocity.x() = 0;
    }

    if (player.jumping) {
        player.velocity.z() = 0.05f;
    } else {
        player.velocity.z() = 0;
    }

    player.position += player.velocity;

    PlayerNode *playerNode = static_cast<PlayerNode *>(node);
    playerNode->setPosition(player.position);
}
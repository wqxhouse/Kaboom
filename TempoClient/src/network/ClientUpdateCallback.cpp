#include "ClientUpdateCallback.h"

#include <osg/Group>

#include "../core/Player.h"
#include "../core/GameStateData.h"

// HACK
Player *findPlayer1(osg::Group *root) {
    return static_cast<Player *>(root->getChild(0));
}

// HACK
Player *findPlayer2(osg::Group *root) {
    return static_cast<Player *>(root->getChild(1));
}

ClientUpdateCallback::ClientUpdateCallback(GameClient *client) : client(client) {
}

ClientUpdateCallback::~ClientUpdateCallback() {
}

void ClientUpdateCallback::operator()(osg::Node *node, osg::NodeVisitor *nv) {
    GameStateUpdateEvent *evt = client->receive();

    if (evt == NULL) {
        return;
    }

    Player *player1 = findPlayer1(static_cast<osg::Group *>(node));
    Player *player2 = findPlayer2(static_cast<osg::Group *>(node));

    player1->setPosition(osg::Vec3(evt->x1, evt->y1, evt->z1));
    player2->setPosition(osg::Vec3(evt->x2, evt->y2, evt->z2));
}

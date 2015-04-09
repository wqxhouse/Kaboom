#include "ClientUpdateCallback.h"

#include <osg/Group>

#include "../core/Player.h"

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

    player1->data->position.x() = evt->x1;
    player1->data->position.y() = evt->y1;
    player1->data->position.z() = evt->z1;

    player2->data->position.x() = evt->x2;
    player2->data->position.y() = evt->y2;
    player2->data->position.z() = evt->z2;
}

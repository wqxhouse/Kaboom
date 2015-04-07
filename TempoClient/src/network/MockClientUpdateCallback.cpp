#include "MockClientUpdateCallback.h"

#include "../core/Player.h"

// HACK
Player *findPlayer(osg::Group *root, int id) {
    return static_cast<Player *>(root->getChild(0));
}

MockClientUpdateCallback::MockClientUpdateCallback(MockClient *client) : client(client) {
}

MockClientUpdateCallback::~MockClientUpdateCallback() {
}

void MockClientUpdateCallback::operator()(osg::Node *node, osg::NodeVisitor *nv) {
    MoveEvent *evt;

    while ((evt = client->recv()) != NULL) {
        Player *player = findPlayer(static_cast<osg::Group *>(node), evt->playerId);

        osg::Vec3 pos = player->data->position;

        if (evt->movingForward) {
            pos.y() += 0.05f;
        }

        if (evt->movingBackward) {
            pos.y() -= 0.05f;
        }

        if (evt->movingLeft) {
            pos.x() -= 0.05f;
        }

        if (evt->movingRight) {
            pos.x() += 0.05f;
        }

        player->setPosition(pos);
    }
}
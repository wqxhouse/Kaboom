#include "ClientUpdateCallback.h"

ClientUpdateCallback::ClientUpdateCallback(Client *client) : client(client) {
}

ClientUpdateCallback::~ClientUpdateCallback() {
}

void ClientUpdateCallback::operator()(osg::Node *node, osg::NodeVisitor *nv) {
    client->receive();
}

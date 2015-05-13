#include "RemoteDetonatorMessageHandler.h"

#include <btBulletDynamicsCommon.h>

#include <core/Entity.h>

#include "Message.h"
#include "MessageType.h"
#include "../components/PhysicsComponent.h"
#include "../messaging/CollisionMessage.h"

bool RemoteDetonatorMessageHandler::handle(const Message &message) const {
    switch (message.getType()) {
        case MessageType::COLLISION: {
            return handle(static_cast<const CollisionMessage &>(message));
        }
    }

    return false;
}

bool RemoteDetonatorMessageHandler::handle(const CollisionMessage &message) const {
    message.getEntity()->getComponent<PhysicsComponent>()->getRigidBody()->setMassProps(0, btVector3(0, 0, 0));
    return true;
}

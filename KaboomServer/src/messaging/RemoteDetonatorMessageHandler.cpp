#include "RemoteDetonatorMessageHandler.h"

#include <btBulletDynamicsCommon.h>

#include <core/Entity.h>

#include "Message.h"
#include "MessageType.h"
#include "../components/PhysicsComponent.h"
#include "../components/StickComponent.h"
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
    Entity *entity = message.getEntity();
    auto &contactEntities = message.getContactEntities();
    auto physComp = entity->getComponent<PhysicsComponent>();
    auto stickComp = entity->getComponent<StickComponent>();

    physComp->getRigidBody()->setMassProps(0.0f, btVector3(0.0f, 0.0f, 0.0f));

    stickComp->setAttached(true);

    if (!contactEntities.empty()) {
        stickComp->setAttachedEntity(*contactEntities.begin());
    }

    return true;
}

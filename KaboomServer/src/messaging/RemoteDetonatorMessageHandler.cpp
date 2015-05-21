#include "RemoteDetonatorMessageHandler.h"

#include <btBulletDynamicsCommon.h>

#include <core/Entity.h>

#include "Message.h"
#include "MessageType.h"
#include "../components/DetonatorComponent.h"
#include "../components/ExplosionComponent.h"
#include "../components/OwnerComponent.h"
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
    Entity *entity = message.getEntity();
    auto &contactEntities = message.getContactEntities();
    auto physComp = entity->getComponent<PhysicsComponent>();
    auto ownerComp = entity->getComponent<OwnerComponent>();
    Entity *owner = ownerComp->getEntity();

    if (contactEntities.empty()) {
        physComp->getRigidBody()->setMassProps(0.0f, btVector3(0.0f, 0.0f, 0.0f));
    } else {
        entity->attachComponent(new ExplosionComponent());

        auto &bombs = owner->getComponent<DetonatorComponent>()->getBombs();
        bombs.erase(std::find(bombs.begin(), bombs.end(), entity));
    }

    return true;
}

#include "KaboomV2MessageHandler.h"

#include <core/Entity.h>

#include "Message.h"
#include "MessageType.h"
#include "../components/ExplosionComponent.h"
#include "../messaging/CollisionMessage.h"

bool KaboomV2MessageHandler::handle(const Message &message) const {
    if (message.getType() != MessageType::COLLISION) {
        return false;
    }

    auto &msg = static_cast<const CollisionMessage &>(message);

    msg.getEntity()->attachComponent(new ExplosionComponent());

    return true;
}

#include "FakeBombMessageHandler.h"

#include <core/Entity.h>

#include "Message.h"
#include "MessageType.h"
#include "../components/ExplosionComponent.h"
#include "../messaging/CollisionMessage.h"

bool FakeBombMessageHandler::handle(const Message &message) const {
    switch (message.getType()) {
        case MessageType::COLLISION: {
            return handle(static_cast<const CollisionMessage &>(message));
        }
    }

    return false;
}

bool FakeBombMessageHandler::handle(const CollisionMessage &message) const {
    message.getEntity()->attachComponent(new ExplosionComponent());
    return true;
}

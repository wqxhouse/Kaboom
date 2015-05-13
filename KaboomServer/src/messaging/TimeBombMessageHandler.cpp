#include "TimeBombMessageHandler.h"

#include <core/Entity.h>

#include "Message.h"
#include "MessageType.h"
#include "TimerExpiredMessage.h"
#include "../components/ExplosionComponent.h"

bool TimeBombMessageHandler::handle(const Message &message) const {
    switch (message.getType()) {
        case MessageType::TIMER_EXPIRED: {
            return handle(static_cast<const TimerExpiredMessage &>(message));
        }
    }

    return false;
}

bool TimeBombMessageHandler::handle(const TimerExpiredMessage &message) const {
    message.getEntity()->attachComponent(new ExplosionComponent());
    return true;
}

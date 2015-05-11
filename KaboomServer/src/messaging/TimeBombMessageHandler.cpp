#include "TimeBombMessageHandler.h"

#include <core/Entity.h>

#include "Message.h"
#include "MessageType.h"
#include "TimerExpiredMessage.h"
#include "../components/ExplosionComponent.h"

bool TimeBombMessageHandler::handle(const Message &message) const {
    if (message.getType() != MessageType::TIMER_EXPIRED) {
        return false;
    }

    auto &msg = static_cast<const TimerExpiredMessage &>(message);

    msg.getEntity()->attachComponent(new ExplosionComponent());

    return true;
}

#pragma once

#include <components/Component.h>

#include "../messaging/MessageHandler.h"

class MessageHandlerComponent : public Component {
public:
    MessageHandlerComponent(MessageHandler *handler)
            : handler(handler) {
    }

    inline MessageHandler *getHandler() const {
        return handler;
    }

private:
    MessageHandler *handler;
};

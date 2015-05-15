#pragma once

#include <components/Component.h>

class MessageHandler;

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

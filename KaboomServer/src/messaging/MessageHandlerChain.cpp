#include "MessageHandlerChain.h"

#include "MessageHandler.h"

void MessageHandlerChain::addHandler(MessageHandler *handler) {
    handlers.push_back(handler);
}

bool MessageHandlerChain::handle(const Message &message) const {
    for (MessageHandler *handler : handlers) {
        if (handler->handle(message)) {
            return true;
        }
    }

    return false;
}

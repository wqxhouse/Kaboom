#include "ChatEventHandler.h"

#include <network/ChatEvent.h>

#include "../core/Game.h"

ChatEventHandler::ChatEventHandler(Game *game)
        : game(game) {
}

void ChatEventHandler::handle(const Event &e) const {
    const ChatEvent &evt = static_cast<const ChatEvent &>(e);

    // TODO: Handle chat event
    printf("%s\n", evt.getMessage());
}

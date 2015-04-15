#pragma once

#include <network/EventHandlerLookup.h>

#include "../core/Game.h"

class ClientEventHandlerLookup : public EventHandlerLookup {
public:
    ClientEventHandlerLookup(Game *game);
    ~ClientEventHandlerLookup();
};

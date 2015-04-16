#pragma once

#include <network/EventHandlerLookup.h>

#include "../core/Game.h"

class Game;

class ServerEventHandlerLookup : public EventHandlerLookup {
public:
    ServerEventHandlerLookup(Game *game);
    ~ServerEventHandlerLookup();
};

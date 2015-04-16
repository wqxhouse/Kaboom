#pragma once

#include <network/EventHandlerLookup.h>

#include "../core/Game.h"

class Game;

class ClientEventHandlerLookup : public EventHandlerLookup {
public:
    ClientEventHandlerLookup(Game *game);
    ~ClientEventHandlerLookup();
};

#pragma once

#include <network/EventHandlerLookup.h>

class Game;

class ClientEventHandlerLookup : public EventHandlerLookup {
public:
    ClientEventHandlerLookup(Game *game);
    ~ClientEventHandlerLookup();
};

#pragma once

#include <network/EventHandlerLookup.h>

class Game;

class ServerEventHandlerLookup : public EventHandlerLookup {
public:
    ServerEventHandlerLookup(Game *game);
    ~ServerEventHandlerLookup();
};

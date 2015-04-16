#pragma once

#include "../core/GameStateData.h"

#include "ClientEventHandlerLookup.h"
#include "ClientNetwork.h"
#include "NetworkData.h"

class GameClient {
public:
    GameClient(ConfigSettings *, ClientEventHandlerLookup *eventHandlerLookup);
    ~GameClient();

    ClientNetwork *network;

    char networkData[MAX_PACKET_SIZE];

	GameStateData *receive();

    void sendMoveEvent(bool movingForward, bool movingBackward, bool movingLeft, bool movingRight);

private:
    ClientEventHandlerLookup *eventHandlerLookup;
};

#pragma once

#include <network/Event.h>
#include <network/PlayerInputEvent.h>
#include <network/PlayerSpawnEvent.h>
#include <network/PositionEvent.h>
#include <network/RotationEvent.h>

#include "../core/GameStateData.h"

#include "ClientEventHandlerLookup.h"
#include "ClientNetwork.h"
#include "NetworkData.h"

#include <network/Event.h>
#include <network/PlayerInputEvent.h>
#include <network/PlayerSpawnEvent.h>
#include <network/PositionEvent.h>
#include <network/RotationEvent.h>


class GameClient {
public:
    GameClient(ConfigSettings *, ClientEventHandlerLookup *eventHandlerLookup);
    ~GameClient();

    ClientNetwork *network;

    char networkData[MAX_PACKET_SIZE];

	void *receive();

    void sendMoveEvent(bool movingForward, bool movingBackward, bool movingLeft, bool movingRight);

private:
    ClientEventHandlerLookup *eventHandlerLookup;
};

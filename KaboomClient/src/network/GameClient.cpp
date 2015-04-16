#include "GameClient.h"

#include <network/EmptyEvent.h>
#include <network/PlayerInputEvent.h>
#include <network/PlayerSpawnEvent.h>
#include <network/PositionEvent.h>
#include <network/RotationEvent.h>

#include "NetworkServices.h"

GameClient::GameClient(ConfigSettings *config, ClientEventHandlerLookup *eventHandlerLookup) :
    eventHandlerLookup(eventHandlerLookup) {
    network = new ClientNetwork(config);
}

GameClient::~GameClient() {
    delete network;
}

void *GameClient::receive() {
    int len = network->receivePackets(networkData);

    if (len <= 0) {
        return nullptr;
    }

    EmptyEvent emptyEvent;
    PositionEvent positionEvent;
    RotationEvent rotationEvent;
    PlayerSpawnEvent playerSpawnEvent;
    PlayerInputEvent playerInputEvent;
	
	printf("received len %d\n", len);

	unsigned int i = 0;
	while (i < (unsigned int)len) {
		emptyEvent.deserialize(&networkData[i]);

		printf("eventType is %d\n", emptyEvent.getOpcode());
		printf("byteSize is %d\n", emptyEvent.getByteSize());

        switch (emptyEvent.getOpcode()) {
        case EventOpcode::POSITION:
            positionEvent.deserialize(&(networkData[i]));
            eventHandlerLookup->find(emptyEvent.getOpcode())->handle(positionEvent);
            break;
        case EventOpcode::ROTATION:
            rotationEvent.deserialize(&(networkData[i]));
            eventHandlerLookup->find(emptyEvent.getOpcode())->handle(rotationEvent);
            break;
        case EventOpcode::PLAYER_INPUT:
            playerInputEvent.deserialize(&(networkData[i]));
            eventHandlerLookup->find(emptyEvent.getOpcode())->handle(playerInputEvent);
            break;
		case EventOpcode::PLAYER_SPAWN:
			playerSpawnEvent.deserialize(&(networkData[i]));
			eventHandlerLookup->find(emptyEvent.getOpcode())->handle(playerSpawnEvent);
			break;
		default:
			printf("error in packet event types\n");
			return nullptr;
		}

		i += emptyEvent.getByteSize();
	}

	return nullptr;
}

void GameClient::sendMessage(const Event &evt) {
    const int &size = evt.getByteSize();
    char *data = new char[size];

    evt.serialize(data);

    NetworkServices::sendMessage(network->clientSocket, data, size);

    delete data;
}

#include "GameClient.h"

#include <network/EmptyEvent.h>
#include <network/PlayerInputEvent.h>
#include <network/EntitySpawnEvent.h>
#include <network/PositionEvent.h>
#include <network/RotationEvent.h>

#include "NetworkServices.h"
#include "ClientEventHandlerLookup.h"

GameClient::GameClient(ConfigSettings *config, ClientEventHandlerLookup *eventHandlerLookup) :
    eventHandlerLookup(eventHandlerLookup) {
    network = new ClientNetwork(config);
}

GameClient::~GameClient() {
    delete network;
}

void GameClient::connectToServer()
{
	network->connectToServer();
}

void GameClient::disconnectFromServer()
{
	network->disconnectFromServer();
}

void GameClient::receive() {
    int len = network->receivePackets(networkData);

    if (len <= 0) {
        return;
    }

    EmptyEvent emptyEvent;
    PositionEvent positionEvent;
    RotationEvent rotationEvent;
	EntitySpawnEvent entitySpawnEvent;
    PlayerInputEvent playerInputEvent;
	
	printf("received len %d\n", len);

	unsigned int i = 0;
	while (i < (unsigned int)len) {
		emptyEvent.deserialize(&networkData[i]);

		printf("eventType is %d\n", emptyEvent.getOpcode());
		printf("byteSize is %d\n", emptyEvent.getByteSize());

        switch (emptyEvent.getOpcode()) {
        case EventOpcode::POSITION:
            positionEvent.deserialize(&networkData[i]);
            eventHandlerLookup->find(emptyEvent.getOpcode())->handle(positionEvent);
            break;
        case EventOpcode::ROTATION:
            rotationEvent.deserialize(&networkData[i]);
            eventHandlerLookup->find(emptyEvent.getOpcode())->handle(rotationEvent);
            break;
        case EventOpcode::PLAYER_INPUT:
            playerInputEvent.deserialize(&networkData[i]);
            eventHandlerLookup->find(emptyEvent.getOpcode())->handle(playerInputEvent);
            break;
		case EventOpcode::ENTITY_SPAWN:
			printf("debug\n");
			entitySpawnEvent.deserialize(&networkData[i]);
			printf("debug2\n");
			eventHandlerLookup->find(emptyEvent.getOpcode())->handle(entitySpawnEvent);

			if (!initialized) {
				currentPlayerEntityId = entitySpawnEvent.getPlayerId(); //set entityId the client needs to keep track of
			}

			break;
		default:
			printf("error in packet event types\n");
			return;
		}

		i += emptyEvent.getByteSize();
	}
}

void GameClient::sendMessage(const Event &evt) {
    const int &size = evt.getByteSize();
    char *data = new char[size];

    evt.serialize(data);

    NetworkServices::sendMessage(network->clientSocket, data, size);

    delete data;
}

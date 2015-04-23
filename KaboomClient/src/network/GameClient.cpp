#include "GameClient.h"

#include <network/EmptyEvent.h>
#include <network/PlayerInputEvent.h>
#include <network/PositionEvent.h>
#include <network/RotationEvent.h>
#include <network/SpawnEvent.h>

#include "NetworkServices.h"
#include "ClientEventHandlerLookup.h"

GameClient::GameClient(ClientEventHandlerLookup *eventHandlerLookup)
    : eventHandlerLookup(eventHandlerLookup) {
}

GameClient::~GameClient() {
}

bool GameClient::connectToServer(const std::string &serverAddress, const int serverPort) {
    network.connectToServer(serverAddress, serverPort);
	return network.isConnected();
	
}

bool GameClient::disconnectFromServer() {
    network.disconnectFromServer();
	return !network.isConnected();
}

void GameClient::receive() {
    char networkData[MAX_PACKET_SIZE];

    int len = network.receivePackets(networkData);

    if (len <= 0) {
        return;
    }

    EmptyEvent emptyEvent;
    PositionEvent positionEvent;
    RotationEvent rotationEvent;
    SpawnEvent spawnEvent;
    PlayerInputEvent playerInputEvent;

    // printf("received len %d\n", len);

    unsigned int i = 0;
    while (i < (unsigned int)len) {
        emptyEvent.deserialize(&networkData[i]);

		/*printf("eventType is %d\n", emptyEvent.getOpcode());
		printf("byteSize is %d\n", emptyEvent.getByteSize());*/

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
            spawnEvent.deserialize(&networkData[i]);
            eventHandlerLookup->find(emptyEvent.getOpcode())->handle(spawnEvent);

            if (!initialized) {
                currentPlayerEntityId = spawnEvent.getEntityId(); //set entityId the client needs to keep track of
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

    network.sendMessage(data, size);

    delete data;
}

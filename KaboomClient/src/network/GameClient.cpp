#include "GameClient.h" 

#include "NetworkServices.h"

GameClient::GameClient(ConfigSettings *config, ClientEventHandlerLookup *eventHandlerLookup) :
    eventHandlerLookup(eventHandlerLookup) {
    network = new ClientNetwork(config);
}

GameClient::~GameClient() {
    delete network;
}

void *GameClient::receive() {
    //Packet packet;

    int len = network->receivePackets(networkData);

    if (len <= 0) {
        return nullptr;
    }

	Event emptyEvent = Event();
	PlayerSpawnEvent playerSpawnEvent = PlayerSpawnEvent();
	PlayerInputEvent playerInputEvent = PlayerInputEvent();
	PositionEvent positionEvent = PositionEvent();
	RotationEvent rotationEvent = RotationEvent();

	
	printf("received len %d\n", len);

	unsigned int i = 0;
	while (i < (unsigned int)len) {
		emptyEvent.deserialize(&(networkData[i]));

		printf("eventType is %d\n", emptyEvent.getOpcode());
		printf("byteSize is %d\n", emptyEvent.getByteSize());


		switch (emptyEvent.getOpcode()) {
		case EventOpcode::PLAYER_SPAWN:

			playerSpawnEvent.deserialize(&(networkData[i]));
			
			eventHandlerLookup->find(emptyEvent.getOpcode())->handle(playerSpawnEvent);
			
			break;

		case EventOpcode::PLAYER_INPUT:
			playerInputEvent.deserialize(&(networkData[i]));
			eventHandlerLookup->find(emptyEvent.getOpcode())->handle(playerInputEvent);
			break;
		case EventOpcode::POSITION:
			positionEvent.deserialize(&(networkData[i]));
			eventHandlerLookup->find(emptyEvent.getOpcode())->handle(positionEvent);

			break;
		case EventOpcode::ROTATION:
			rotationEvent.deserialize(&(networkData[i]));
			eventHandlerLookup->find(emptyEvent.getOpcode())->handle(rotationEvent);
			break;
		default:
			printf("error in packet event types\n");
			return nullptr;
		}

		i += emptyEvent.getByteSize();
		//printf("new i is %d\n", i);
	}

	return nullptr;
}

void GameClient::sendMoveEvent(bool movingForward, bool movingBackward, bool movingLeft, bool movingRight) {
    const unsigned int size = sizeof(MoveEvent);
    char data[size];
	cout << "enter the sendMoveEvent" << endl;
    MoveEvent packet;
	packet.packet_type = MOVE_EVENT;
    packet.movingForward = movingForward;
    packet.movingBackward = movingBackward;
    packet.movingLeft = movingLeft;
    packet.movingRight = movingRight;

    packet.serialize(data);

    NetworkServices::sendMessage(network->clientSocket, data, size);
}

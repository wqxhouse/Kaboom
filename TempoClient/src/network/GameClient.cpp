#include "GameClient.h" 

#include "NetworkServices.h"
#include "../core/GameStateData.h"

GameClient::GameClient(ConfigSettings *config) {
    network = new ClientNetwork(config);
}

GameClient::~GameClient() {
    delete network;
}

GameStateUpdateEvent *GameClient::receive() {
    Packet packet;

    int len = network->receivePackets(networkData);

    if (len <= 0) {
        return NULL;
    }

    printf("received len %d", len);
    packet.deserialize(networkData);

    GameStateUpdateEvent *gameStateUpdateEvent;
	GameStateUpdateEventV2 game;
	AssignEvent as;
	switch (packet.packet_type) {
	case ASSIGN_EVENT:
		as.deserialize(networkData);
		printf("\nplayer is %d\n",as.client_id);
		return NULL;
    case GAME_STATE_UPDATE_EVENT:


        gameStateUpdateEvent = new GameStateUpdateEvent();
        //gameStateUpdateEvent->deserialize(networkData);
		//game = new GameStateUpdateEventV2();
		game.deserialize(networkData,len);
		gameStateUpdateEvent->x1 = game.playerList[0].x;
		gameStateUpdateEvent->y1 = game.playerList[0].y;
		gameStateUpdateEvent->z1 = game.playerList[0].z;
		if (game.numOfPlayers != 2){
			gameStateUpdateEvent->x2 = 0;
			gameStateUpdateEvent->y2 = 0;
			gameStateUpdateEvent->z2 = 0;
		}
		else{
			gameStateUpdateEvent->x2 = game.playerList[1].x;
			gameStateUpdateEvent->y2 = game.playerList[1].y;
			gameStateUpdateEvent->z2 = game.playerList[1].z;
		}
        //printf("\nx1 %x\n", gameStateUpdateEvent->x1);
        //printf("y1 %x\n", gameStateUpdateEvent->y1);
        //printf("z1 %x\n", gameStateUpdateEvent->z1);
        //printf("x2 %x\n", gameStateUpdateEvent->x2);
        //printf("y2 %x\n", gameStateUpdateEvent->y2);
        //printf("z2 %x\n", gameStateUpdateEvent->z2);

        return gameStateUpdateEvent;
    default:
        printf("error in packet types\n");
        return NULL;
    }
}

void GameClient::sendMoveEvent(bool movingForward, bool movingBackward, bool movingLeft, bool movingRight) {
    const unsigned int size = sizeof(MoveEvent);
    char data[size];

    MoveEvent packet;
	packet.packet_type = MOVE_EVENT;
    packet.movingForward = movingForward;
    packet.movingBackward = movingBackward;
    packet.movingLeft = movingLeft;
    packet.movingRight = movingRight;
    packet.serialize(data);

    NetworkServices::sendMessage(network->clientSocket, data, size);
}

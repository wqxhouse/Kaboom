#include "GameClient.h" 

#include "NetworkServices.h"

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
    
    switch (packet.packetType) {
    case GAME_STATE_UPDATE_EVENT:

        gameStateUpdateEvent = new GameStateUpdateEvent();
        gameStateUpdateEvent->deserialize(networkData);

        printf("x1 %x\n", gameStateUpdateEvent->x1);
        printf("y1 %x\n", gameStateUpdateEvent->y1);
        printf("z1 %x\n", gameStateUpdateEvent->z1);
        printf("x2 %x\n", gameStateUpdateEvent->x2);
        printf("y2 %x\n", gameStateUpdateEvent->y2);
        printf("z2 %x\n", gameStateUpdateEvent->z2);

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
    packet.packetType = MOVE_EVENT;
    packet.movingForward = movingForward;
    packet.movingBackward = movingBackward;
    packet.movingLeft = movingLeft;
    packet.movingRight = movingRight;
    packet.serialize(data);

    NetworkServices::sendMessage(network->clientSocket, data, size);
}

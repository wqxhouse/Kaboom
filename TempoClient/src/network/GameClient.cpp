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

    packet.deserialize(networkData);

    GameStateUpdateEvent *gameStateUpdateEvent;
    
    switch (packet.packetType) {
    case GAME_STATE_UPDATE_EVENT:
        gameStateUpdateEvent = new GameStateUpdateEvent();
        gameStateUpdateEvent->deserialize(networkData);
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

#include "ServerGame.h"

unsigned int ServerGame::client_id;

ServerGame::ServerGame(ConfigSettings * config) {
    // id's to assign clients for our table
    client_id = 0;

    // set up the server network to listen 
    network = new ServerNetwork(config);

    gameState.packet_type = GAME_STATE_UPDATE_EVENT;
    gameState.x1 = 0;
    gameState.y1 = 0;
    gameState.z1 = 0;
    gameState.x2 = 0;
    gameState.y2 = 0;
    gameState.z2 = 0;


}

void ServerGame::update() {
    // get new clients
    if (network->acceptNewClient(client_id)) {
        printf("<Server> client %d has been connected to the server\n", client_id);
        client_id++;
    }

    receiveFromClients();
}

void ServerGame::receiveFromClients() {
    Packet packet;

    // go through all clients
    std::map<unsigned int, SOCKET>::iterator iter;

    for (iter = network->sessions.begin(); iter != network->sessions.end(); iter++) {
        int data_length = network->receiveData(iter->first, network_data);


        //if data_length == 0, that means the client

        if (data_length <= 0) {
            continue;//no data recieved
        }

        packet.deserialize(network_data);

        MoveEvent moveEvent;

        switch (packet.packet_type) {
        case INIT_CONNECTION:
            printf("<Server> received init packet from client\n");
            break;
        case MOVE_EVENT:
            moveEvent.deserialize(network_data);
            printf("<Server> data length: %d\n", data_length);
            printf("<Server>packet length is %d\n", data_length);

            printf("<Server> data is %x\n", packet);

            printf("back %x\n", moveEvent.movingBackward);
            printf("forward %x\n", moveEvent.movingForward);
            printf("left %x\n", moveEvent.movingLeft);
            printf("right %x\n", moveEvent.movingRight);

            //if (iter->first == 1) {
                if (moveEvent.movingForward) {
                    gameState.y1++;
                } else if (moveEvent.movingBackward) {
                    gameState.y1--;
                }

                if (moveEvent.movingRight) {
                    gameState.x1++;
                } else if (moveEvent.movingLeft) {
                    gameState.x1--;
                }
            /*} else if (iter->first == 2) {
                if (moveEvent.movingForward) {
                    gameState.z2++;
                } else if (moveEvent.movingBackward) {
                    gameState.z2--;
                }

                if (moveEvent.movingRight) {
                    gameState.x2++;
                } else if (moveEvent.movingLeft) {
                    gameState.x2--;
                }
            }*/


            sendGameStatePackets(gameState);

            break;

        default:
            printf("<Server> error in packet types\n");
            break;
        }

    }
}

void ServerGame::sendActionPackets() {
    // send action packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = ACTION_EVENT;

    packet.serialize(packet_data);

    network->sendToAll(packet_data, packet_size);
}

void ServerGame::sendGameStatePackets(GameStateUpdateEvent packet) {
    // send action packet
    const unsigned int packet_size = sizeof(GameStateUpdateEvent);
    char packet_data[packet_size];

    packet.packet_type = GAME_STATE_UPDATE_EVENT;

    packet.serialize(packet_data);

    printf("x1 %x\n", packet.x1);
    printf("y1 %x\n", packet.y1);
    printf("z1 %x\n", packet.z1);
    printf("x2 %x\n", packet.x2);
    printf("y2 %x\n", packet.y2);
    printf("z2 %x\n", packet.z2);

    network->sendToAll(packet_data, packet_size);

}
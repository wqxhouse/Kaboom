#include "ServerGame.h"

unsigned int ServerGame::client_id;

ServerGame::ServerGame(ConfigSettings * config) {
    // id's to assign clients for our table
    client_id = 0;

    // set up the server network to listen 
    network = new ServerNetwork(config);
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

        if (data_length <= 0) {
            continue;//no data recieved
        }

        

        //int i = 0;
        //while (i < (unsigned int)data_length) {
            packet.deserialize(network_data);

            MoveEvent moveEvent;
            //i += sizeof(Packet); //Max Packet Size

            switch (packet.packet_type) {
            case INIT_CONNECTION:
                printf("<Server> received init packet from client\n");
                //printf("<Server> send action packet to client\n");
                //sendActionPackets();
                break;
            case ACTION_EVENT:
                printf("<Server> received action event packet from client\n");
                //printf("<Server> send action packet to client\n");
                //sendActionPackets();
                break;
            case MOVE_EVENT:
                moveEvent.deserialize(network_data);
                printf("<Server> data length: %d\n", data_length);
                printf("<Server>packet length is %d\n", data_length);

                printf("<Server> data is %x\n", packet);
                //MoveEvent * newPacket = static_cast<MoveEvent *>(&packet);

                printf("back %x", moveEvent.movingBackward);
                printf("forward %x", moveEvent.movingForward);
                printf("left %x", moveEvent.movingLeft);
                printf("right %x", moveEvent.movingRight);

                

                break;

            default:
                printf("<Server> error in packet types\n");
                break;
            }
        //}
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
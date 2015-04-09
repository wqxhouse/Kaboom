#include "Client.h" 

Client::Client(ConfigSettings * config) {
    network = new ClientNetwork(config);

    // send init packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = INIT_CONNECTION;

    packet.serialize(packet_data);
    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);

}

void Client::sendActionPackets() {
    // send action packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = ACTION_EVENT;

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void Client::sendMoveEvents() {

    const unsigned int packet_size = sizeof(MoveEvent);
    char packet_data[packet_size];

    MoveEvent packet;
    packet.packet_type = MOVE_EVENT;
    packet.movingBackward = true;
    packet.movingForward = false;
    packet.movingLeft = true;
    packet.movingRight = false;
    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void Client::update() {
    Packet packet;
    int data_length = network->receivePackets(network_data);

    if (data_length <= 0) {
        //no data recieved
        return;
    }

    int i = 0;
    while (i < (unsigned int)data_length) {
        packet.deserialize(&(network_data[i]));
        i += sizeof(Packet);

        switch (packet.packet_type) {

        case ACTION_EVENT:

            printf("client received action event packet from server\n");
            printf("client sending move event packet to server\n");
            sendMoveEvents();

            break;

        default:

            printf("error in packet types\n");

            break;
        }
    }
}
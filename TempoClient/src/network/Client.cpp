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

void Client::receive() {
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
        default:
            printf("error in packet types\n");
            break;
        }
    }
}

void Client::sendMoveEvent(bool movingForward, bool movingBackward, bool movingLeft, bool movingRight) {
    const unsigned int size = sizeof(MoveEvent);
    char data[size];

    MoveEvent packet;
    packet.packet_type = MOVE_EVENT;
    packet.movingForward = movingForward;
    packet.movingBackward = movingBackward;
    packet.movingLeft = movingLeft;
    packet.movingRight = movingRight;
    packet.serialize(data);

    NetworkServices::sendMessage(network->ConnectSocket, data, size);
}

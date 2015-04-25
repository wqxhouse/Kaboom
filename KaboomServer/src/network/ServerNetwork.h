#pragma once

#include <unordered_map>
#include <unordered_set>

#include <Winsock2.h>
#include <ws2tcpip.h>

#include <ConfigSettings.h>
#include <network/NetworkData.h>

#include "NetworkServices.h"

using namespace std;

class ServerNetwork {
public:
    ConfigSettings * config;

    ServerNetwork(ConfigSettings *);
    ~ServerNetwork(void);

    // Socket to listen for new connections
    SOCKET ListenSocket;

    // table to keep track of each client's socket
    std::unordered_map<unsigned int, SOCKET> sessions;

    // accept new connections
    bool acceptNewClient(unsigned int id);

    // receive incoming data
    int receive(unsigned int clientId, char *recvbuf);

    void send(char *packet, int size);
    void send(char *packet, int size, int clientId);

    void removeDisconnectedClients();

private:
    std::unordered_set<unsigned int> disconnectedClients;
};
#pragma once

#include <unordered_map>
#include <unordered_set>

#include <network/NetworkData.h>
#include <util/IdPool.h>

#include "NetworkServices.h"

class ConfigSettings;

class ServerNetwork {
public:
    typedef std::unordered_set<unsigned int> IdSet;

    ServerNetwork(ConfigSettings *config);

    bool acceptClient(unsigned int &playerId);

    int receive(unsigned int playerId, char *recvbuf);

    void send(char *packet, int size);
    void send(char *packet, int size, int playerId);

    inline const IdSet &getDisconnectedPlayerIds() const {
        return disconnectedPlayerIds;
    }

    void removeDisconnectedPlayers();

private:
    typedef std::unordered_map<unsigned int, SOCKET> IdToSocketMap;

    ConfigSettings *config;

    SOCKET listenSocket;

    IdPool playerIdPool;
    IdToSocketMap sessions;
    IdSet disconnectedPlayerIds;

    void send(char *packet, int size, unsigned int playerId, SOCKET socket);
};
#pragma once

#include <Winsock2.h>

#include <unordered_map>
#include <unordered_set>

#include <util/IdPool.h>

class ConfigSettings;

class ServerNetwork {
public:
    typedef std::unordered_set<unsigned int> IdSet;

    ServerNetwork(ConfigSettings *config);

    bool acceptClient(unsigned int &playerId);

    int receive(unsigned int playerId, char *recvbuf, int bufSize);

    void send(char *packet, int size);
    void send(char *packet, int size, int playerId);

    inline const IdSet &getDisconnectedPlayerIds() const {
        return disconnectedPlayerIds;
    }

    void removeDisconnectedPlayers();

private:
    typedef std::unordered_map<unsigned int, SOCKET> IdToSocketMap;

    SOCKET listenSocket;

    IdPool playerIdPool;
    IdToSocketMap sessions;
    IdSet disconnectedPlayerIds;

    void send(char *packet, int size, unsigned int playerId, SOCKET socket);
};
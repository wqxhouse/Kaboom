#pragma once

#include <network/Event.h>
#include <network/NetworkData.h>
#include "ClientNetwork.h"

class ClientEventHandlerLookup;

class GameClient {
public:
    GameClient(const ClientEventHandlerLookup &eventHandlerLookup);
    ~GameClient();

	void receive();
    bool connectToServer(const std::string &serverAddress, const int serverPort);
	bool disconnectFromServer();

    void sendMessage(const Event &evt) const;

	unsigned int getCurrentPlayerEntityId() const;

	bool getAssignedEntity() const;
	bool getIsConnectedToServer() const;
private:
    const ClientEventHandlerLookup &eventHandlerLookup;

    ClientNetwork network;

    bool assignedEntity;

	unsigned int currentPlayerEntityId;

};

#pragma once

#include <network/Event.h>
#include <network/HealthEvent.h>
#include <network/NetworkData.h>
#include "ClientNetwork.h"

class ClientEventHandlerLookup;
enum EntityType;

class GameClient {
public:
    GameClient(const ClientEventHandlerLookup &eventHandlerLookup);

	void receive();
    bool connectToServer(const std::string &serverAddress, const int serverPort);
	bool disconnectFromServer();

    void sendMessage(const Event &evt) const;

    void sendPlayerRenameEvent(const std::string &name) const;
    void sendPlayerSelectionEvent(EntityType type) const;
    void sendEquipEvent(EntityType type) const;
    void sendReloadRequestEvent() const;
	void sendPlayerRespawnRequestEvent() const;

	bool getIsConnectedToServer() const;

private:
    const ClientEventHandlerLookup &eventHandlerLookup;

    ClientNetwork network;

};

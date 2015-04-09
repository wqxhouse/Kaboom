#include "ServerNetwork.h"
#include "NetworkData.h"
#include "NetworkServices.h"

#include "../core/GameStateData.h"

class ServerGame
{

public:

	ServerGame(ConfigSettings *);
	~ServerGame(void);

	void update();

	void receiveFromClients();

	void sendActionPackets();

    void sendMovePackets();

    void sendGameStatePackets(GameStateUpdateEvent);

	void sendAssignPackets(int);

private:

	// IDs for the clients connecting for table in ServerNetwork 
	static unsigned int client_id;

	// The ServerNetwork object 
	ServerNetwork* network;

	// data buffer
	char network_data[MAX_PACKET_SIZE];

    GameStateUpdateEvent gameState;

	std::map<int, int> playerIndexNum;

};
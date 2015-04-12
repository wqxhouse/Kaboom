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

    void sendGameStatePackets();

	void sendAssignPackets(int);

private:

	// IDs for the clients connecting for table in ServerNetwork 
	static unsigned int client_id;

	// The ServerNetwork object 
	ServerNetwork* network;

	// data buffer
	char network_data[MAX_PACKET_SIZE];

	// This is the game state data the server keep tracks of
	GameStateData gameStateData;


	// This is the game state data we send over the network
    GameStateUpdateEvent gameStateUpdateEvent;

	std::map<int, int> playerIndexNum;

};
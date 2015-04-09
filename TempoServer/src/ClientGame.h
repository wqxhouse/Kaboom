#include <winsock2.h>
#include <Windows.h>
#include "ClientNetwork.h"
#include "NetworkData.h"
#include "NetworkServices.h"

class ClientGame
{

public:

	ClientGame(ConfigSettings *);
	~ClientGame(void);

	ClientNetwork* network;

    void sendActionPackets();

    char network_data[MAX_PACKET_SIZE];

    void update();

    void sendMoveEvents();

};
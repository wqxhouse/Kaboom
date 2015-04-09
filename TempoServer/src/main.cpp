#include "network/ServerGame.h"
// used for multi-threading
#include <process.h>

void serverLoop();

ServerGame * server;

int main()
{
	ConfigSettings* config = ConfigSettings::config;
	// initialize the server
	server = new ServerGame(config);

    serverLoop();
}

void serverLoop() {
	while (true)
	{
		server->update();
	}
}
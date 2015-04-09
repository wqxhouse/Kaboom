#pragma once

#include <iostream>
#include <enet/enet.h>
#include <string>
#include "util/ConfigSettings.h"
#include <iostream>
#include "Event.h"


class Server {
public: 
	ENetAddress address;
	ENetHost * server;
	ConfigSettings* config;

	int createServer();
	int getUserInput();
	Server(ConfigSettings*);
	~Server();

};
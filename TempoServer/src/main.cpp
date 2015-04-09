#include "ServerGame.h"
#include "ClientGame.h"
// used for multi-threading
#include <process.h>

void serverLoop(void *);
void clientLoop(void);

ServerGame * server;
ClientGame * client;

int main()
{
	ConfigSettings* config = ConfigSettings::config;
	// initialize the server
	server = new ServerGame(config);
	// create thread with arbitrary argument for the run function
	_beginthread(serverLoop, 0, NULL);

	// initialize the client 
	//Sleep(1000);
	
	client = new ClientGame(config);
	
	clientLoop();
}

void serverLoop(void * arg)
{
	while (true)
	{
		server->update();
		
	}
}

void clientLoop()
{
	while (true)
	{
		//do game stuff
		//will later run client->update();
        client->update();
	}
}




/*#include <iostream>
#include <enet/enet.h>
#include <string>
#include "util/ConfigSettings.h"
#include "core/Server.h"
#include <chrono>
int main(int argc, char ** argv) {
	
	//Load the Server Config File
	ConfigSettings* config = ConfigSettings::config;

	//Initialize Server Class, and setup the server
	Server * serverObj = new Server(config);
	serverObj->createServer();

	int tickTime;
	config->getValue(ConfigSettings::str_tick_time, tickTime);

	using namespace std::chrono;
	int startTick = duration_cast< milliseconds >(
		high_resolution_clock::now().time_since_epoch()
	).count();
	int currentTick = 0;
	bool logic = false;
	while (true){
		currentTick = duration_cast<milliseconds>(
			high_resolution_clock::now().time_since_epoch()
			).count() - startTick;

		if (currentTick <= (tickTime / 2)) {
			logic = false; //grab user inputs and connections
			



		}
		else if(logic){
			logic = true; 
			
		}
		else{
			//if done with everything wait for next round of inputs
			Sleep(tickTime - currentTick);
			startTick = duration_cast< milliseconds >(
				high_resolution_clock::now().time_since_epoch()
				).count();
		}
	}

/*
	ENetEvent event;
	//Wait up to 1000 milliseconds for an event.
	int bb = 0;
	while (1) {
		char *fdsa;
		ENetPacket * packet;
		while (enet_host_service(server, &event, 0) > 0 && tick) {
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT:

				printf("A new client connected from %x:%u.\n",
					event.peer->address.host,
					event.peer->address.port);
				//Store any relevant client information here.
				fdsa = new char[16];
				sprintf_s(fdsa, 16, "%d", bb++);
				event.peer->data = fdsa;

				break;
			case ENET_EVENT_TYPE_RECEIVE:
				printf("receive\n");
				printf("A packet of length %u containing %s was received from %s on channel %u.\n",
					event.packet->dataLength,
					event.packet->data,
					event.peer->data,
					event.channelID);
				//Clean up the packet now that we're done using it.
				enet_packet_destroy(event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				printf("disconnect\n");
				break;
			default:
				break;
			}
		}
		//Now broadcast the game state to all the clients


		Sleep(5000);

		printf("Tick tock.\n");
	}

	enet_host_destroy(server);
}*/
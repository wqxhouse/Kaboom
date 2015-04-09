#include "Server.h"

Server::Server(ConfigSettings* _config){
	config = _config;

	//Initialize Enet library
	if (enet_initialize() != 0) {
		fprintf(stderr, "An error occurred while initializing ENet.\n");
		//return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);
}

Server::~Server() { }

int Server::createServer(){
	int port;
	int maxClient;
	int channel;
	int incomingBandwidth;
	int outgoingBandwidth;

	config->getValue(ConfigSettings::str_server_port, port);
	config->getValue(ConfigSettings::str_max_client, maxClient);
	config->getValue(ConfigSettings::str_channel, channel);
	config->getValue(ConfigSettings::str_incoming_bandwidth, incomingBandwidth);
	config->getValue(ConfigSettings::str_outgoing_bandwidth, outgoingBandwidth);

	//Bind this server to a specific port
	/* enet_address_set_host (& address, "x.x.x.x"); */
	address.host = 0;
	/* Bind the server to port */
	address.port = port;
	server = enet_host_create(&address /* the address to bind the server host to */,
		maxClient			   /* allow up to 32 clients and/or outgoing connections */,
		channel				   /* allow up to 2 channels to be used, 0 and 1 */,
		incomingBandwidth      /* assume any amount of incoming bandwidth */,
		outgoingBandwidth      /* assume any amount of outgoing bandwidth */);
	if (server == NULL) {
		fprintf(stderr,
			"An error occurred while trying to create an ENet server host.\n");
		exit(EXIT_FAILURE);
	}
}
/*
int Server::getUserInput(){

	char *fdsa;
	ENetPacket * packet;
	ENetEvent event;
	int bb = 0;

	while (enet_host_service(server, &event, 0) > 0) {
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
			printf("A packet of length %u containing [%s] was received from %s on channel %u.\n",
				event.packet->dataLength,
				event.packet->data,
				event.peer->data,
				event.channelID);
			//Clean up the packet now that we're done using it.

			int eventNum;
			memcpy(&eventNum, event.packet->data, 4);
			switch (eventNum){
			case 0:
				break;
			case 1:
				struct MoveClientEvent mce;
				memcpy(&mce, 4 + event.packet->data, sizeof(MoveClientEvent));
				//handle move command being sent with even.peer->data to recognize the character
				break;
			case 2:
				break;
			case 3:
				break;
			default:
				break;
			}
			enet_packet_destroy(event.packet);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			printf("disconnect\n");
			break;
		default:
			break;
		}
	}
}*/



#include "GameServer.h"

unsigned int GameServer::client_id;

GameServer::GameServer(ConfigSettings * config) {
    // id's to assign clients for our table
    client_id = 0;

	printf("<Server> Creating a Network Server\n");
    // set up the server network to listen 
    network = new ServerNetwork(config);


	printf("<Sevrer> Initializing a Game\n");
	int maxPlayers;
	config->getValue(ConfigSettings::str_max_client, maxPlayers);

	//gameStateUpdateEvent.packet_type = GAME_STATE_UPDATE_EVENT; TODO

}

GameServer::~GameServer() {
}

bool GameServer::acceptNewClient() {
    if (network->acceptNewClient(client_id)) {
        printf("<Server> Client %d has connected to the server\n", client_id);

        client_id++;

        return true;
    }

    return false;
}
/*
void GameServer::update() {
    // get new clients
    if (network->acceptNewClient(client_id)) {

		//check if maxPlayer is reached TODO
		gameStateData.connectedPlayers += 1;
		PlayerData playerData;

		//spawn point, and initial velocity TODO
		playerData.client_id = client_id;
		playerData.x = playerData.y = playerData.z = playerData.vx = playerData.vy = playerData.vz = 1.1;

		gameStateData.playerDataList.push_back(playerData);

		NetworkPlayerData netPlayerData;
		netPlayerData.client_id = client_id;
		netPlayerData.x = netPlayerData.y = netPlayerData.z = 1.1;
		
		gameStateUpdateEvent.addToPlayerList(netPlayerData);

		//cout << gameStateUpdateEvent.playerList[0].x << endl;
		//printf("*** x %f\n", gameState.playerList[0].x);
		//printf("*** y %f\n", gameState.playerList[0].y);
		//printf("*** z %f\n", gameState.playerList[0].z);
		
		sendAssignPackets(client_id);

        client_id++;
    }

    receiveFromClients();

	//updateGameLogic 
}
*/
void GameServer::receiveFromClients(Game *game) {
    Packet packet;

    // go through all clients
    std::map<unsigned int, SOCKET>::iterator iter;

    for (iter = network->sessions.begin(); iter != network->sessions.end(); iter++) {
        int data_length = network->receiveData(iter->first, network_data);

		MoveEvent moveEvent;
		if (data_length <= 0) {
			//TODO if data_length == 0, client disconnected, need to update game state information about the client
			continue;//no data recieved
		}

		printf("len %d\n", data_length);
		int i = 0;
		while (i < (unsigned int)data_length) {

            ServerPlayer *player = nullptr;
			printf("received len %d\n", data_length);
			packet.deserialize(&(network_data[i]));


			switch (packet.packet_type) {
			case INIT_CONNECTION:
				printf("<Server> received init packet from client\n");
				break;
			case MOVE_EVENT:
				moveEvent.deserialize(&(network_data[i]));

				//gameStateData.playerDataList[iter->first].setMove(moveEvent);

				//printf("<Server> data length: %d\n", data_length);
				//printf("<Server>packet length is %d\n", data_length);

				printf("<Server> Received move event\n");

				//printf("client_id %d\n", iter->first);
				//printf("back %d\n", moveEvent.movingBackward);
				//printf("forward %d\n", moveEvent.movingForward);
				//printf("left %d\n", moveEvent.movingLeft);
				//printf("right %d\n", moveEvent.movingRight);

				if (iter->first == 0) {
                    player = game->players[0];
                } else if (iter->first == 1) {
                    player = game->players[1];
                }

                if (moveEvent.movingForward) {
                    player->setVelocityY(1);
                } else if (moveEvent.movingBackward) {
                    player->setVelocityY(-1);
                }

                if (moveEvent.movingRight) {
                    player->setVelocityX(1);
                } else if (moveEvent.movingLeft) {
                    player->setVelocityX(-1);
                }

				break;

			default:
				printf("<Server> error in packet types\n");
				break;
			}
			i += packet.packet_size;
			printf("new i is %d\n", i);
		}
    }
}

void GameServer::sendActionPackets() {
    // send action packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = ASSIGN_EVENT;

    packet.serialize(packet_data);

    network->sendToAll(packet_data, packet_size);
}

void GameServer::sendGameStatePackets(Game *game) {
    gameStateUpdateEvent.numOfPlayers = game->players.size();

	for (ServerPlayer* player : game->players) {
		sendPlayerPositionEvent(player);
	}
	/*
    // send action packet
    const unsigned int packet_size = sizeof(GameStateUpdateEvent);
    char packet_data[packet_size];

	gameStateUpdateEvent.serialize(packet_data);

    network->sendToAll(packet_data, packet_size);
	*/
	//send position, send input
}

void GameServer::sendAssignPackets(int client_id) {
	const unsigned int packet_size = sizeof(AssignEvent);
	char packet_data[packet_size];

	AssignEvent packet;
	packet.packet_type = ASSIGN_EVENT;
	packet.client_id = client_id;
	packet.serialize(packet_data);

	network->sendToOneClient(packet_data,packet_size,client_id);
}


void GameServer::sendPlayerSpawnEvent(ServerPlayer* serverPlayer) {

	PlayerSpawnEvent playerSpawnEvent(serverPlayer->getId(), serverPlayer->getX(), serverPlayer->getY(), serverPlayer->getZ());

	const unsigned int packet_size = sizeof(PlayerSpawnEvent);
	char packet_data[packet_size];

	playerSpawnEvent.serialize(packet_data);
	network->sendToAll(packet_data, packet_size);
}

void GameServer::sendPlayerPositionEvent(ServerPlayer* serverPlayer) {
	PositionEvent positionEvent(serverPlayer->getId(), serverPlayer->getX(), serverPlayer->getY(), serverPlayer->getZ());

	const unsigned int packet_size = sizeof(PositionEvent);
	char packet_data[packet_size];

	positionEvent.serialize(packet_data);
	network->sendToAll(packet_data, packet_size);

}
/*
void GameServer::sendPlayerInputEvent(ServerPlayer* serverPlayer, MoveEvent * moveEvent) {

	PlayerInputEvent

}*/
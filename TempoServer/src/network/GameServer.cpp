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

	gameStateData.connectedPlayers = 0;
	gameStateData.maxPlayers = maxPlayers;
	//gameStateData.playerDataList.resize(maxPlayers);


	//gameStateUpdateEvent.packet_type = GAME_STATE_UPDATE_EVENT; TODO

}

GameServer::~GameServer() {
}

void GameServer::update() {
    // get new clients
    if (network->acceptNewClient(client_id)) {
        printf("<Server> client %d has been connected to the server\n", client_id);

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

void GameServer::receiveFromClients() {
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

			printf("received len %d\n", data_length);
			packet.deserialize(&(network_data[i]));


			switch (packet.packet_type) {
			case INIT_CONNECTION:
				printf("<Server> received init packet from client\n");
				break;
			case MOVE_EVENT:
				moveEvent.deserialize(&(network_data[i]));

				gameStateData.playerDataList[iter->first].setMove(moveEvent);

				//printf("<Server> data length: %d\n", data_length);
				//printf("<Server>packet length is %d\n", data_length);

				printf("<Server> data is %x\n", packet);

				cout << iter->first << endl;
				cout << moveEvent.movingBackward << endl;
				cout << moveEvent.movingForward << endl;
				cout << moveEvent.movingLeft << endl;
				cout << moveEvent.movingRight << endl;

				//printf("client_id %d\n", iter->first);
				//printf("back %d\n", moveEvent.movingBackward);
				//printf("forward %d\n", moveEvent.movingForward);
				//printf("left %d\n", moveEvent.movingLeft);
				//printf("right %d\n", moveEvent.movingRight);
				cout << "*******enter move event"<< endl;
				if (iter->first == 0) {
					cout << "*******enter move event part 2" << endl;
					if (moveEvent.movingForward) {
						gameStateData.playerDataList[0].y+=1;
						cout << "*******enter move event part 3" << endl;
					}
					else if (moveEvent.movingBackward) {
						gameStateData.playerDataList[0].y--;
						cout << "*******enter move event part 3" << endl;
					}

					if (moveEvent.movingRight) {
						gameStateData.playerDataList[0].x++;
						cout << "*******enter move event part 3" << endl;
					}
					else if (moveEvent.movingLeft) {
						gameStateData.playerDataList[0].x--;
						cout << "*******enter move event part 3" << endl;
					}
				}
				else if (iter->first == 1) {
					if (moveEvent.movingForward) {
						gameStateData.playerDataList[1].y++;
					}
					else if (moveEvent.movingBackward) {
						gameStateData.playerDataList[1].y--;
					}

					if (moveEvent.movingRight) {
						gameStateData.playerDataList[1].x++;
					}
					else if (moveEvent.movingLeft) {
						gameStateData.playerDataList[1].x--;
					}
				}

				
				sendGameStatePackets();

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

void GameServer::sendGameStatePackets() {

	gameStateUpdateEvent.numOfPlayers = gameStateData.connectedPlayers;

	for (int i = 0; i < gameStateData.playerDataList.size(); i++){
		gameStateUpdateEvent.playerList[i].client_id = gameStateData.playerDataList[i].client_id;
		cout << i << endl;
		cout << "client id " << gameStateUpdateEvent.playerList[i].client_id << endl;
		cout << "client id " << gameStateData.playerDataList[i].client_id << endl;
		gameStateUpdateEvent.playerList[i].x = gameStateData.playerDataList[i].x;
		gameStateUpdateEvent.playerList[i].y = gameStateData.playerDataList[i].y;
		gameStateUpdateEvent.playerList[i].z = gameStateData.playerDataList[i].z;
		cout << "x " << gameStateData.playerDataList[i].x << endl;
		cout << "y " << gameStateData.playerDataList[i].y << endl;
		cout << "z " << gameStateData.playerDataList[i].z << endl;
		cout << "x " << gameStateUpdateEvent.playerList[i].x << endl;
		cout << "y " << gameStateUpdateEvent.playerList[i].y << endl;
		cout << "z " << gameStateUpdateEvent.playerList[i].z << endl;
	}

    // send action packet
    const unsigned int packet_size = sizeof(GameStateUpdateEvent);
    char packet_data[packet_size];

	gameStateUpdateEvent.serialize(packet_data);

    //printf("x1 %x\n", packet.x1);
    //printf("y1 %x\n", packet.y1);
    //printf("z1 %x\n", packet.z1);
    //printf("x2 %x\n", packet.x2);
    //printf("y2 %x\n", packet.y2);
    //printf("z2 %x\n", packet.z2);

    network->sendToAll(packet_data, packet_size);

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
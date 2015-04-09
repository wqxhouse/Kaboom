#pragma once

#include "../network/NetworkData.h"
#include "PlayerData.h"

struct GameStateUpdateEvent : Packet {
	int packetSize=8;

	int numOfPlayers=0;
	PlayerData playerList[4];
	//	int numOfBombs;
	//	std::vector<Bomb> bombList;

	void serialize(char * data) {
		printf("serialize size %d\n",packetSize);
		printf("size of playerData%d\n", sizeof(PlayerData));
		memcpy(data, this, packetSize);
	}
	void deserialize(char * data) {
		memcpy(this,data,packetSize);
		/*memcpy(data, &packetSize, 4);
		memcpy(data + 4, &numOfPlayers, 4);
		for (int i = 0; i < numOfPlayers; i++){
			memcpy(data + 8 + i*(sizeof(AlmostPlayerData)), &playerList[i], sizeof(AlmostPlayerData));
		}*/
	}

	void addToPlayerList(PlayerData playerData){
		playerList[numOfPlayers] = playerData;
		numOfPlayers++;
		packetSize = packetSize + sizeof(PlayerData);
	}
};
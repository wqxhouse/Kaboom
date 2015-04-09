#pragma once

#include "../network/NetworkData.h"
#include "PlayerData.h"

struct GameStateUpdateEvent : Packet {
	//	int packetSize;

	//	int numOfPlayers;
	//	std::vector<PlayerInfo> playerList;
	//	int numOfBombs;
	//	std::vector<Bomb> bombList;

	float x1;
	float y1;
	float z1;

	float x2;
	float y2;
	float z2;

	void serialize(char * data) {
		memcpy(data, this, sizeof(GameStateUpdateEvent));
	}
	void deserialize(char * data) {
		memcpy(this, data, sizeof(GameStateUpdateEvent));
	}
};
/*
struct PlayerInfo {
int client_id;
float x;
float y;
float z;




};*/
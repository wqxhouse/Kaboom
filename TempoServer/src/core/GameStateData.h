#pragma once

#include "../network/NetworkData.h"
#include "PlayerData.h"


struct GameStateData {


	int maxPlayers;
	int connectedPlayers;
	std::vector<PlayerData> playerDataList;


	//std::vector<BombData> bombDataList;

};




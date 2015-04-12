#pragma once

#include "../network/NetworkData.h"

struct PlayerData {
    int client_id;

    float x;
    float y;
    float z;


	float vx;
	float vy;
	float vz;


	bool movingForward;
	bool movingBackward;
	bool movingLeft;
	bool movingRight;
	
	bool fired;
	bool jumping;


	//look perspective


	void setMove(MoveEvent moveEvent) {
		movingBackward = moveEvent.movingBackward;
		movingForward = moveEvent.movingForward;
		movingLeft = moveEvent.movingLeft;
		movingRight = moveEvent.movingRight;
	}
	void setFire(FireEvent fireEvent){
		fired = fireEvent.fired;
	}
	void setJump(JumpEvent jumpEvent){
		jumping = jumpEvent.jumping;
	}
};
#pragma once

#include <string.h>

#define MAX_PACKET_SIZE 1000000

enum PacketTypes {

    INIT_CONNECTION = 0,

    ASSIGN_EVENT = 1,

    MOVE_EVENT = 2,

    FIRE_EVENT = 3,

    LOOK_EVENT = 4,

    JUMP_EVENT = 5,

    GAME_STATE_UPDATE_EVENT = 6,

};

struct Packet {

    unsigned int packet_type;

	void serialize(char * data) {
        memcpy(data, this, sizeof(Packet));
	}

	void deserialize(char * data) {
        memcpy(this, data, sizeof(Packet));
	}
};

struct AssignEvent : Packet{
	int client_id;
	void serialize(char * data) {
		//printf("serialize size of MoveEvent : %d\n", sizeof(MoveEvent));
		memcpy(data, this, sizeof(AssignEvent));
	}
	void deserialize(char * data) {
		//printf("deserialize size of MoveEvent : %d\n", sizeof(MoveEvent));
		memcpy(this, data, sizeof(AssignEvent));
	}
};
struct MoveEvent : Packet {
    bool movingForward;
    bool movingBackward;
    bool movingLeft;
    bool movingRight;

    void serialize(char * data) {
        //printf("serialize size of MoveEvent : %d\n", sizeof(MoveEvent));
        memcpy(data, this, sizeof(MoveEvent));
    }
    void deserialize(char * data) {
        //printf("deserialize size of MoveEvent : %d\n", sizeof(MoveEvent));
        memcpy(this, data, sizeof(MoveEvent));
    }

};

struct FireEvent : Packet {
    bool fired;
    void serialize(char * data) {
        //printf("size of FireEvent : %d", sizeof(FireEvent));
        memcpy(data, this, sizeof(FireEvent));
    }
    void deserialize(char * data) {
        memcpy(this, data, sizeof(FireEvent));
    }
};

struct LookEvent : Packet {
    float x;
    float y;
    float z;
    void serialize(char * data) {
        //printf("size of LookEvent : %d", sizeof(LookEvent));
        memcpy(data, this, sizeof(LookEvent));
    }
    void deserialize(char * data) {
        memcpy(this, data, sizeof(LookEvent));
    }
};

struct JumpEvent : Packet {
    bool jumping;
    void serialize(char * data) {
        //printf("size of jumping : %d", sizeof(JumpEvent));
        memcpy(data, this, sizeof(JumpEvent));
    }
    void deserialize(char * data) {
        memcpy(this, data, sizeof(JumpEvent));
    }
};
struct AlmostPlayerData{
	int client_id;

	float x;
	float y;
	float z;
};

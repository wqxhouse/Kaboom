/*struct Event {
	EventType eventType;
	//int playerId;
	void serialize(Event data){
		memcpy(&eventType, data, 4);
		size_t size = 0;
		switch (eventType){
			case FIRE_EVENT:
				size = sizeof(FireEvent);
				break;
			case MOVE_EVENT:
				size = sizeof(MoveEvent);
				break;
			case LOOK_EVENT:
				size = sizeof(LookEvent);
				break;
			case JUMP_EVENT:
				size = sizeof(JumpEvent);
				break;
		}
		memcpy(data, 4+this, size);
	}

	void deserialize(char * data) {
		memcpy(&eventType, data, 4);
		size_t size = 0;
		switch (eventType){
		case FIRE_EVENT:
			size = sizeof(FireEvent);
			break;
		case MOVE_EVENT:
			size = sizeof(MoveEvent);
			break;
		case LOOK_EVENT:
			size = sizeof(LookEvent);
			break;
		case JUMP_EVENT:
			size = sizeof(JumpEvent);
			break;
		}
		memcpy(this, data, 4+size);
	}
};

struct FireEvent : Event {
	bool fire;
};

struct MoveEvent : Event {
	bool movingForward;
	bool movingBackward;
	bool movingLeft;
	bool movingRight;
};

struct LookEvent : Event {
	float x;
	float y;
	float z;
};

struct JumpEvent : Event {
	bool jumping;
};

typedef enum _EventType
{
   MOVE_EVENT = 0,  
   LOOK_EVENT = 1,  
   JUMP_EVENT = 2,  
   FIRE_EVENT = 3
} EventType;

*/
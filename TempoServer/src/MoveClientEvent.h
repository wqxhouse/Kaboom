//first 4 bytes are equal to 1
struct MoveClientEvent {
	bool movingForward;
	bool movingBackward;
	bool movingLeft;
	bool movingRight;
};
#pragma once

#include <osg/Vec3>

class Player {
public:
	static void moveForward();
	static void stopMoveForward();
	static void moveBackward();
	static void stopMoveBackward();
	static void moveLeft();
	static void stopMoveLeft();
	static void moveRight();
	static void stopMoveRight();
	static void jump();
	static void stopJump();

	bool movingForward;
	bool movingBackward;
	bool movingLeft;
	bool movingRight;
	bool jumping;

	osg::Vec3 position;
	osg::Vec3 velocity;

	Player();
	~Player();
};

#pragma once

#include <osg/Vec3>

class Player {
public:
	static void moveForwardDown();
	static void moveForwardUp();
	static void moveBackwardDown();
	static void moveBackwardUp();
	static void moveLeftDown();
	static void moveLeftUp();
	static void moveRightDown();
	static void moveRightUp();
	static void jumpDown();
	static void jumpUp();

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

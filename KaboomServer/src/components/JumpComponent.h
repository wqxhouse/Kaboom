#pragma once

#include <components/Component.h>

class Entity;

class JumpComponent: public Component {
public:
	JumpComponent();
	~JumpComponent();

	void setJumping(bool tf);
	bool isJumping();

private:
	bool _isJumping;
};

#include "JumpComponent.h"

JumpComponent::JumpComponent()
	: _isJumping(false)
{
}

JumpComponent::~JumpComponent()
{
}

void JumpComponent::setJumping(bool tf)
{
	_isJumping = tf;
}

bool JumpComponent::isJumping()
{
	return _isJumping;
}
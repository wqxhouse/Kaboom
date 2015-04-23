#include "InputEventHandler.h"

#include <Core.h>
#include <network/PlayerInputEvent.h>
#include <osg/io_utils>

#include "../network/GameClient.h"

InputEventHandler::InputEventHandler(GameClient &client)
    : client(client) {
}

InputEventHandler::~InputEventHandler() {
}

void InputEventHandler::onMoveForwardDown() {
    movingForward = true;
    sendPlayerInputEvent();

	_velocity.x() += _kSpeed * Core::getLastFrameDuration();
	updateLocalCamera();
}

void InputEventHandler::onMoveForwardUp() {
    movingForward = false;
    sendPlayerInputEvent();

	_velocity.x() = 0;
	updateLocalCamera();
}

void InputEventHandler::onMoveBackwardDown() {
    movingBackward = true;
    sendPlayerInputEvent();

	_velocity.x() -= _kSpeed * Core::getLastFrameDuration();
	updateLocalCamera();
}

void InputEventHandler::onMoveBackwardUp() {
    movingBackward = false;
    sendPlayerInputEvent();

	_velocity.x() = 0;
	updateLocalCamera();
}

void InputEventHandler::onMoveLeftDown() {
    movingLeft = true;
    sendPlayerInputEvent();

	_velocity.y() += _kSpeed * Core::getLastFrameDuration();
	updateLocalCamera();
}

void InputEventHandler::onMoveLeftUp() {
    movingLeft = false;
    sendPlayerInputEvent();

	_velocity.y() = 0;
	updateLocalCamera();
}

void InputEventHandler::onMoveRightDown() {
    movingRight = true;
    sendPlayerInputEvent();

	_velocity.y() -= _kSpeed * Core::getLastFrameDuration();
	updateLocalCamera();
}

void InputEventHandler::onMoveRightUp() {
    movingRight = false;
    sendPlayerInputEvent();

	_velocity.y() = 0;
	updateLocalCamera();
}

void InputEventHandler::onJumpDown() {
    jumping = true;
    sendPlayerInputEvent();
}

void InputEventHandler::onJumpUp() {
    jumping = false;
    sendPlayerInputEvent();
}

void InputEventHandler::onFireDown() {
	firing = true;
	sendPlayerInputEvent();
}

void InputEventHandler::onFireUp() {
	firing = false;
	sendPlayerInputEvent();
}

void InputEventHandler::onLook(float delta_yaw, float delta_pitch) {
	this->yaw += delta_yaw;
	this->pitch += delta_pitch;

	if (this->pitch < -89) this->pitch = -89;
	if (this->pitch > 89) this->pitch = 89;
    sendPlayerInputEvent();
	updateLocalCamera();
}

void InputEventHandler::updateLocalCamera()
{
	// compute local camera
	osg::Quat q0;
	osg::Quat q1;

	q0.makeRotate(osg::DegreesToRadians(-yaw), 0, 0, 1);
	q1.makeRotate(osg::DegreesToRadians(pitch), 1, 0, 0);

	osg::Matrix orientation;
	(q1 * q0).get(orientation);
	osg::Vec3 dir = orientation.preMult(osg::Vec3(0, 1, 0));

	osg::Vec3 x, y, z;
	x = dir;
	y = dir ^ osg::Vec3(0, 0, 1);
	y.normalize();
	z = y ^ x;

	osg::Vec3 eye = Core::getMainCamera().getEyePosition();
	eye = eye + (x * _velocity.x() + y * _velocity.y() + z * _velocity.z()) * Core::getLastFrameDuration();
	osg::Vec3 lookAt = eye + dir;
	osg::Vec3 up(0, 0, 1);
	
	Core::getMainCamera().setViewAndUpdate(eye, lookAt, up);

	// TODO: the following doesn't need to be updated for every event
	// but need to set as soon as the game mode is on
	// TODO: put this two as initial values in the config file
	Core::getMainCamera().setFovYAndUpdate(89);
	Core::getMainCamera().setNearAndFar(1, 500); // TODO: check if this will work with computer_near_far in osg
}

void InputEventHandler::sendPlayerInputEvent() {
    PlayerInputEvent evt(0,
        movingForward,
        movingBackward,
        movingLeft,
        movingRight,
        jumping,
        firing,
        yaw,
        pitch);

    client.sendMessage(evt);
}

void InputEventHandler::enterGameMode()
{
	Core::enableGameMode();
}

void InputEventHandler::quitGameMode()
{
	Core::disableGameMode();
}

void InputEventHandler::showDebugAnalysis()
{
	Core::enablePassDataDisplay();
}

void InputEventHandler::hideDebugAnalysis()
{
	Core::disablePassDataDisplay();
}
/*
void InputManager::look(int deltaX, int deltaY) {
	Camera* cam_ptr = &Core::getMainCamera();
    osg::Vec3 lookAt = cam_ptr->getLookAt();
	printf("before: %f, %f, %f\n", lookAt[0], lookAt[1], lookAt[2]);
	lookAt = lookAt + osg::Vec3(deltaX / 100.0, 0, deltaY / 100.0);
	printf("after: %f, %f, %f\n", lookAt[0], lookAt[1], lookAt[2]);
	cam_ptr->setLookAtAndUpdate(lookAt);
	viewer.getCamera()->setViewMatrix(cam_ptr->getViewMatrix());
}

void InputManager::resetCamera()
{
	const osg::Vec3 eye(0, -10, 0);
	const osg::Vec3 center(0, 1, 0);
	Core::getMainCamera().setEyePositionAndUpdate(eye);
	Core::getMainCamera().setLookAtAndUpdate(center);
	Core::getMainCamera().setUpAndUpdate(osg::Vec3(0, 0, 1));
	viewer.getCamera()->setViewMatrix(Core::getMainCamera().getViewMatrix());
}
*/

/*
void InputManager::loadConfig() {
    keyboardHandler->bindKey('w', KeyboardEventHandler::KEY_DOWN, InputManager::moveForwardDown);
    keyboardHandler->bindKey('w', KeyboardEventHandler::KEY_UP, InputManager::moveForwardUp);
    keyboardHandler->bindKey('s', KeyboardEventHandler::KEY_DOWN, InputManager::moveBackwardDown);
    keyboardHandler->bindKey('s', KeyboardEventHandler::KEY_UP, InputManager::moveBackwardUp);
    keyboardHandler->bindKey('a', KeyboardEventHandler::KEY_DOWN, InputManager::moveLeftDown);
    keyboardHandler->bindKey('a', KeyboardEventHandler::KEY_UP, InputManager::moveLeftUp);
    keyboardHandler->bindKey('d', KeyboardEventHandler::KEY_DOWN, InputManager::moveRightDown);
    keyboardHandler->bindKey('d', KeyboardEventHandler::KEY_UP, InputManager::moveRightUp);
    keyboardHandler->bindKey(' ', KeyboardEventHandler::KEY_DOWN, InputManager::jumpDown);
    keyboardHandler->bindKey(' ', KeyboardEventHandler::KEY_UP, InputManager::jumpUp);
	keyboardHandler->bindKey('f', KeyboardEventHandler::KEY_DOWN, InputManager::firingDown);
	keyboardHandler->bindKey('f', KeyboardEventHandler::KEY_UP, InputManager::firingUp);

	// editor related
	keyboardHandler->bindKey(osgGA::GUIEventAdapter::KEY_Escape, KeyboardEventHandler::KEY_UP, InputManager::quitGameMode);
	keyboardHandler->bindKey(osgGA::GUIEventAdapter::KEY_F9, KeyboardEventHandler::KEY_UP, InputManager::showDebugAnalysis);
	keyboardHandler->bindKey(osgGA::GUIEventAdapter::KEY_F10, KeyboardEventHandler::KEY_UP, InputManager::hideDebugAnalysis);

	// reset camera
	keyboardHandler->bindKey('r', KeyboardEventHandler::KEY_DOWN, InputManager::resetCamera);
}
*/

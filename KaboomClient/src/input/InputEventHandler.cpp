#include "InputEventHandler.h"

#include <Core.h>
#include <network/PlayerInputEvent.h>

#include "../network/GameClient.h"

InputEventHandler::InputEventHandler(GameClient &client)
    : client(client) {
}

InputEventHandler::~InputEventHandler() {
}

void InputEventHandler::onMoveForwardDown() {
    movingForward = true;
    sendPlayerInputEvent();
}

void InputEventHandler::onMoveForwardUp() {
    movingForward = false;
    sendPlayerInputEvent();
}

void InputEventHandler::onMoveBackwardDown() {
    movingBackward = true;
    sendPlayerInputEvent();
}

void InputEventHandler::onMoveBackwardUp() {
    movingBackward = false;
    sendPlayerInputEvent();
}

void InputEventHandler::onMoveLeftDown() {
    movingLeft = true;
    sendPlayerInputEvent();
}

void InputEventHandler::onMoveLeftUp() {
    movingLeft = false;
    sendPlayerInputEvent();
}

void InputEventHandler::onMoveRightDown() {
    movingRight = true;
    sendPlayerInputEvent();
}

void InputEventHandler::onMoveRightUp() {
    movingRight = false;
    sendPlayerInputEvent();
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

void InputEventHandler::onLook(float yaw, float pitch) {
    this->yaw = yaw;
    this->pitch = pitch;
    sendPlayerInputEvent();
}

void InputEventHandler::computeCamera(int dx, int dy)
{
	osg::Quat q;
	
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

#include "InputManager.h"

#include "../Global.h"

bool InputManager::movingForward;
bool InputManager::movingBackward;
bool InputManager::movingLeft;
bool InputManager::movingRight;
bool InputManager::jumping;

void InputManager::moveForwardDown() {
    movingForward = true;
    g_client.sendMoveEvent(movingForward, movingBackward, movingLeft, movingRight);
}

void InputManager::moveForwardUp() {
    movingForward = false;
    g_client.sendMoveEvent(movingForward, movingBackward, movingLeft, movingRight);
}

void InputManager::moveBackwardDown() {
    movingBackward = true;
    g_client.sendMoveEvent(movingForward, movingBackward, movingLeft, movingRight);
}

void InputManager::moveBackwardUp() {
    movingBackward = false;
    g_client.sendMoveEvent(movingForward, movingBackward, movingLeft, movingRight);
}

void InputManager::moveLeftDown() {
    movingLeft = true;
    g_client.sendMoveEvent(movingForward, movingBackward, movingLeft, movingRight);
}

void InputManager::moveLeftUp() {
    movingLeft = false;
    g_client.sendMoveEvent(movingForward, movingBackward, movingLeft, movingRight);
}

void InputManager::moveRightDown() {
    movingRight = true;
    g_client.sendMoveEvent(movingForward, movingBackward, movingLeft, movingRight);
}

void InputManager::moveRightUp() {
    movingRight = false;
    g_client.sendMoveEvent(movingForward, movingBackward, movingLeft, movingRight);
}

void InputManager::jumpDown() {
    jumping = true;
}

void InputManager::jumpUp() {
    jumping = false;
}

InputManager::InputManager(osgViewer::Viewer *viewer) {
    keyboardHandler = new KeyboardEventHandler();
    viewer->addEventHandler(keyboardHandler);
}

InputManager::~InputManager() {
}

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
}

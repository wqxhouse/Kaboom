#include "InputManager.h"

#include "../network/GameClient.h"

InputManager::InputManager(GameClient &client)
    : inputEventHandler(client),
    keyboardEventHandler(inputEventHandler),
    mouseEventHandler(inputEventHandler) {
}

InputManager::~InputManager() {
}

void InputManager::loadConfig() {
    keyboardEventHandler.bindKey('w', KeyboardEventHandler::KEY_DOWN, &InputEventHandler::onMoveForwardDown);
    keyboardEventHandler.bindKey('w', KeyboardEventHandler::KEY_UP, &InputEventHandler::onMoveForwardUp);
    keyboardEventHandler.bindKey('s', KeyboardEventHandler::KEY_DOWN, &InputEventHandler::onMoveBackwardDown);
    keyboardEventHandler.bindKey('s', KeyboardEventHandler::KEY_UP, &InputEventHandler::onMoveBackwardUp);
    keyboardEventHandler.bindKey('a', KeyboardEventHandler::KEY_DOWN, &InputEventHandler::onMoveLeftDown);
    keyboardEventHandler.bindKey('a', KeyboardEventHandler::KEY_UP, &InputEventHandler::onMoveLeftUp);
    keyboardEventHandler.bindKey('d', KeyboardEventHandler::KEY_DOWN, &InputEventHandler::onMoveRightDown);
    keyboardEventHandler.bindKey('d', KeyboardEventHandler::KEY_UP, &InputEventHandler::onMoveRightUp);
    keyboardEventHandler.bindKey(' ', KeyboardEventHandler::KEY_DOWN, &InputEventHandler::onJumpDown);
    keyboardEventHandler.bindKey(' ', KeyboardEventHandler::KEY_UP, &InputEventHandler::onJumpUp);
    keyboardEventHandler.bindKey('f', KeyboardEventHandler::KEY_DOWN, &InputEventHandler::onFireDown);
    keyboardEventHandler.bindKey('f', KeyboardEventHandler::KEY_UP, &InputEventHandler::onFireUp);
}

KeyboardEventHandler &InputManager::getKeyboardEventHandler() {
    return keyboardEventHandler;
}

MouseEventHandler &InputManager::getMouseEventHandler() {
    return mouseEventHandler;
}

#include "InputManager.h"

#include "../network/GameClient.h"

InputManager::InputManager(GameClient &client)
        : inputEventHandler(client),
          keyboardEventHandler(inputEventHandler),
          mouseEventHandler(inputEventHandler) {
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
    mouseEventHandler.bindKey(osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON, MouseEventHandler::KEY_DOWN, &InputEventHandler::onAttack1Down);
    mouseEventHandler.bindKey(osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON, MouseEventHandler::KEY_UP, &InputEventHandler::onAttack1Up);
    mouseEventHandler.bindKey(osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON, MouseEventHandler::KEY_DOWN, &InputEventHandler::onAttack2Down);
    mouseEventHandler.bindKey(osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON, MouseEventHandler::KEY_UP, &InputEventHandler::onAttack2Up);
	keyboardEventHandler.bindKey(osgGA::GUIEventAdapter::KEY_Escape, KeyboardEventHandler::KEY_UP, &InputEventHandler::quitGameMode);

	// editor related
	//keyboardEventHandler.bindKey(osgGA::GUIEventAdapter::KEY_F8, KeyboardEventHandler::KEY_UP, &InputEventHandler::enterGameMode);
	//keyboardEventHandler.bindKey(osgGA::GUIEventAdapter::KEY_F9, KeyboardEventHandler::KEY_UP, &InputEventHandler::showDebugAnalysis);
	//keyboardEventHandler.bindKey(osgGA::GUIEventAdapter::KEY_F10, KeyboardEventHandler::KEY_UP, &InputEventHandler::hideDebugAnalysis);
}

KeyboardEventHandler &InputManager::getKeyboardEventHandler() {
    return keyboardEventHandler;
}

MouseEventHandler &InputManager::getMouseEventHandler() {
    return mouseEventHandler;
}

#include "InputManager.h"

#include "../network/GameClient.h"

InputManager::InputManager(GameClient &client, Game * game)
        : inputEventHandler(client, game),
          keyboardEventHandler(inputEventHandler),
          mouseEventHandler(inputEventHandler) {
}

void InputManager::loadConfig() {
	keyboardEventHandler.unbindKeyAll();
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
	keyboardEventHandler.bindKey(osgGA::GUIEventAdapter::KEY_Tab, KeyboardEventHandler::KEY_DOWN, &InputEventHandler::onTab);
	keyboardEventHandler.bindKey(osgGA::GUIEventAdapter::KEY_Tab, KeyboardEventHandler::KEY_UP, &InputEventHandler::offTab);
    mouseEventHandler.bindKey(osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON, MouseEventHandler::KEY_DOWN, &InputEventHandler::onAttack1Down);
    mouseEventHandler.bindKey(osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON, MouseEventHandler::KEY_UP, &InputEventHandler::onAttack1Up);
    mouseEventHandler.bindKey(osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON, MouseEventHandler::KEY_DOWN, &InputEventHandler::onAttack2Down);
    mouseEventHandler.bindKey(osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON, MouseEventHandler::KEY_UP, &InputEventHandler::onAttack2Up);
    keyboardEventHandler.bindKey('1', &InputEventHandler::onEquip1);
    keyboardEventHandler.bindKey('2', &InputEventHandler::onEquip2);
    keyboardEventHandler.bindKey('3', &InputEventHandler::onEquip3);
    keyboardEventHandler.bindKey('4', &InputEventHandler::onEquip4);
    keyboardEventHandler.bindKey('5', &InputEventHandler::onEquip5);
    keyboardEventHandler.bindKey(osgGA::GUIEventAdapter::KEY_F5, &InputEventHandler::onReloadRequest);
	keyboardEventHandler.bindKey(osgGA::GUIEventAdapter::KEY_Escape, KeyboardEventHandler::KEY_UP, &InputEventHandler::quitGameMode);

	// editor related
	//keyboardEventHandler.bindKey(osgGA::GUIEventAdapter::KEY_F8, KeyboardEventHandler::KEY_UP, &InputEventHandler::enterGameMode);
	//keyboardEventHandler.bindKey(osgGA::GUIEventAdapter::KEY_F9, KeyboardEventHandler::KEY_UP, &InputEventHandler::showDebugAnalysis);
	//keyboardEventHandler.bindKey(osgGA::GUIEventAdapter::KEY_F10, KeyboardEventHandler::KEY_UP, &InputEventHandler::hideDebugAnalysis);
}

void InputManager::loadNameTyping()
{
	keyboardEventHandler.unbindKeyAll();
}

KeyboardEventHandler &InputManager::getKeyboardEventHandler() {
    return keyboardEventHandler;
}

MouseEventHandler &InputManager::getMouseEventHandler() {
    return mouseEventHandler;
}

#include "InputEventHandler.h"

#include <Core.h>
#include <core/EntityType.h>
#include <network/PlayerInputEvent.h>

#include "../network/GameClient.h"

InputEventHandler::InputEventHandler(GameClient &client, Game * game)
        : client(client), _game(game){
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

void InputEventHandler::onAttack1Down() {
    attacking1 = true;
    sendPlayerInputEvent();
}

void InputEventHandler::onAttack1Up() {
    attacking1 = false;
    sendPlayerInputEvent();
}

void InputEventHandler::onAttack2Down() {
    attacking2 = true;
    sendPlayerInputEvent();
}

void InputEventHandler::onAttack2Up() {
    attacking2 = false;
    sendPlayerInputEvent();
}

void InputEventHandler::onLook(float delta_yaw, float delta_pitch) {
    this->yaw += delta_yaw;
    this->pitch += delta_pitch;

    if (this->pitch < -89) {
        this->pitch = -89;
    }

    if (this->pitch > 89) {
        this->pitch = 89;
    }

    sendPlayerInputEvent();

    Core::getMainCamera().setYawAndPitchAndUpdate(yaw, pitch);
}

void InputEventHandler::onEquip1() {
    client.sendEquipEvent(KABOOM_V2);
	_game->getGameGUIEventHandler()->changeWeapon(0);
}

void InputEventHandler::onEquip2() {
    client.sendEquipEvent(TIME_BOMB);
	_game->getGameGUIEventHandler()->changeWeapon(1);
}

void InputEventHandler::onEquip3() {
    client.sendEquipEvent(REMOTE_DETONATOR);
	_game->getGameGUIEventHandler()->changeWeapon(2);
}

void InputEventHandler::typeCharacter(char c)
{
	_game->name->push_back(c);
	_game->getGameGUIEventHandler()->updateUserName(_game->name);
}

void InputEventHandler::removeCharacter()
{
	if (_game->name->size() > 0)
	{
		_game->name->pop_back();
		_game->getGameGUIEventHandler()->updateUserName(_game->name);
	}
}

void InputEventHandler::sendPlayerInputEvent() {
    PlayerInputEvent evt(
            0,
            movingForward,
            movingBackward,
            movingLeft,
            movingRight,
            jumping,
            attacking1,
            attacking2,
            yaw,
            pitch);

    client.sendMessage(evt);
}
void InputEventHandler::onTab(){

	_game->getGameGUIEventHandler()->showScoreBoard();
}
void InputEventHandler::offTab(){
	_game->getGameGUIEventHandler()->hideScoreBoard();
}

void InputEventHandler::onReloadRequest() {
    client.sendReloadRequestEvent();
}

//void InputEventHandler::enterGameMode() {
//	Core::enableGameMode();
//}

void InputEventHandler::quitGameMode() {
	Core::disableGameMode();
}
//
//void InputEventHandler::showDebugAnalysis() {
//	Core::enablePassDataDisplay();
//}
//
//void InputEventHandler::hideDebugAnalysis() {
//	Core::disablePassDataDisplay();
//}

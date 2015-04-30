#include "InputEventHandler.h"

#include <Core.h>
#include <network/PlayerInputEvent.h>

#include "../network/GameClient.h"

InputEventHandler::InputEventHandler(GameClient &client)
        : client(client) {
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

#pragma once

#include "KaboomClient\src\core\Game.h"

class GameClient;

class InputEventHandler {
public:
    InputEventHandler(GameClient &client, Game * game);

    void onMoveForwardDown();
    void onMoveForwardUp();
    void onMoveBackwardDown();
    void onMoveBackwardUp();
    void onMoveLeftDown();
    void onMoveLeftUp();
    void onMoveRightDown();
    void onMoveRightUp();
    void onJumpDown();
    void onJumpUp();
    void onAttack1Down();
    void onAttack1Up();
    void onAttack2Down();
    void onAttack2Up();
    void onLook(float yaw, float pitch);
    void onEquip1();
    void onEquip2();
    void onEquip3();
    void onEquip4();
    void onEquip5();
	void onTab();
	void offTab();
	void typeCharacter(char c);
	void removeCharacter();

    void onReloadRequest();

    // void enterGameMode();
    void quitGameMode();
    // void showDebugAnalysis();
    // void hideDebugAnalysis();

private:
    GameClient &client;
	Game * _game;

    bool movingForward;
    bool movingBackward;
    bool movingLeft;
    bool movingRight;
    bool jumping;
    bool attacking1;
    bool attacking2;
    float yaw;
    float pitch;

    void sendPlayerInputEvent();
};

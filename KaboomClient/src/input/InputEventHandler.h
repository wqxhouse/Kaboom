#pragma once

class GameClient;

class InputEventHandler {
public:
    InputEventHandler(GameClient &client);
    ~InputEventHandler();

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
    void onFireDown();
    void onFireUp();
    void onLook(float yaw, float pitch);

private:
    GameClient &client;

    bool movingForward;
    bool movingBackward;
    bool movingLeft;
    bool movingRight;
    bool jumping;
    bool firing;
    float yaw;
    float pitch;

    void sendPlayerInputEvent();

    //static void quitGameMode();
    //static void showDebugAnalysis();
    //static void hideDebugAnalysis();

    //static void resetCamera();
};

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

	void enterGameMode();
	void quitGameMode();
	void showDebugAnalysis();
	void hideDebugAnalysis();

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

	osg::Vec3 _velocity;
	const float _kSpeed = 100;

    void sendPlayerInputEvent();
	void updateLocalCamera();

    //static void resetCamera();
};

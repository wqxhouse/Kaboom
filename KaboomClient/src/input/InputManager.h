#pragma once

#include <osgViewer/Viewer>
#include "KeyboardEventHandler.h"

class InputManager {
public:
    static void look(int deltaX, int deltaY);

    InputManager(osgViewer::Viewer *viewer);
    ~InputManager();

    void loadConfig();

private:
    static bool movingForward;
    static bool movingBackward;
    static bool movingLeft;
    static bool movingRight;
    static bool jumping;

    static void moveForwardDown();
    static void moveForwardUp();
    static void moveBackwardDown();
    static void moveBackwardUp();
    static void moveLeftDown();
    static void moveLeftUp();
    static void moveRightDown();
    static void moveRightUp();
    static void jumpDown();
    static void jumpUp();

    static void sendPlayerInputEvent();

    osg::ref_ptr<KeyboardEventHandler> keyboardHandler;
};

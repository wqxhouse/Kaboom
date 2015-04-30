#pragma once

#include "InputEventHandler.h"
#include "KeyboardEventHandler.h"
#include "MouseEventHandler.h"

class GameClient;

class InputManager {
public:
    InputManager(GameClient &client);

    void loadConfig();

    KeyboardEventHandler &getKeyboardEventHandler();
    MouseEventHandler &getMouseEventHandler();

private:
    InputEventHandler inputEventHandler;
    KeyboardEventHandler keyboardEventHandler;
    MouseEventHandler mouseEventHandler;
};

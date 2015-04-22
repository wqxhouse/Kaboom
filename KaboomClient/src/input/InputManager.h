#pragma once

#include "InputEventHandler.h"
#include "KeyboardEventHandler.h"
#include "MouseEventHandler.h"
#include "../network/GameClient.h"

class InputManager {
public:
    InputManager(GameClient &client);
    ~InputManager();

    void loadConfig();

private:
    InputEventHandler inputEventHandler;
    KeyboardEventHandler keyboardEventHandler;
    MouseEventHandler mouseEventHandler;
};

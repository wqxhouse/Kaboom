#pragma once

#include "InputEventHandler.h"
#include "KeyboardEventHandler.h"
#include "MouseEventHandler.h"
#include "KaboomClient\src\core\Game.h"

class GameClient;

class InputManager {
public:
    InputManager(GameClient &client, Game * game);

    void loadConfig();

    KeyboardEventHandler &getKeyboardEventHandler();
    MouseEventHandler &getMouseEventHandler();

private:
    InputEventHandler inputEventHandler;
    KeyboardEventHandler keyboardEventHandler;
    MouseEventHandler mouseEventHandler;
};

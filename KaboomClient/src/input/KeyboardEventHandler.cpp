#include "KeyboardEventHandler.h"

#include <iostream>

#include "InputManager.h"

bool KeyboardEventHandler::bindKey(int key, Function func) {
    if (keyDownFuncMap.end() != keyDownFuncMap.find(key)) {
        std::cout << "Duplicate key '" << key << "' ignored." << std::endl;
        return false;
    } else {
        keyDownFuncMap[key].keyFunction = func;
        return true;
    }
}

bool KeyboardEventHandler::bindKey(int key, KeyState state, Function func) {
    if (state == KEY_DOWN) {
        return bindKey(key, func);
    } else {
        if (keyUpFuncMap.end() != keyUpFuncMap.find(key)) {
            std::cout << "Duplicate key '" << key << "' ignored." << std::endl;
            return false;
        } else {
            keyUpFuncMap[key].keyFunction = func;
            return true;
        }
    }
}

bool KeyboardEventHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {
    KeyFunctionMap::iterator itr;
    bool newKeyDownEvent = false;
    bool newKeyUpEvent = false;

    int centerx = (ea.getWindowX() + ea.getWindowWidth()) / 2;
    int centery = (ea.getWindowY() + ea.getWindowHeight()) / 2;
    int x = ea.getX();
    int y = ea.getY();
    int deltaX = x - prevX;
    int deltaY = y - prevY;

    switch (ea.getEventType()) {
    case osgGA::GUIEventAdapter::KEYDOWN:
        itr = keyDownFuncMap.find(ea.getKey());

        if (itr != keyDownFuncMap.end()) {
            if (itr->second.keyState == KEY_UP) {
                itr->second.keyState = KEY_DOWN;
                newKeyDownEvent = true;
            }

            if (newKeyDownEvent) {
                itr->second.keyFunction();
                newKeyDownEvent = false;
            }

            return true;
        }

        return false;
    case osgGA::GUIEventAdapter::KEYUP:
        itr = keyDownFuncMap.find(ea.getKey());

        if (itr != keyDownFuncMap.end()) {
            itr->second.keyState = KEY_UP;
        }

        itr = keyUpFuncMap.find(ea.getKey());

        if (itr != keyUpFuncMap.end()) {
            itr->second.keyFunction();
            return true;
        }

        return false;
    case osgGA::GUIEventAdapter::MOVE:
        if (deltaX != 0 || deltaY != 0) {
            InputManager::look(deltaX, deltaY);

            prevX = x;
            prevY = y;
            //aa.requestWarpPointer(centerx, centery);
        }
        return false;
	
    default:
        return false;
    }
}

void KeyboardEventHandler::accept(osgGA::GUIEventHandlerVisitor &v) {
    v.visit(*this);
};

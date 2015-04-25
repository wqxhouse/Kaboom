#include "KeyboardEventHandler.h"

#include <iostream>

KeyboardEventHandler::KeyboardEventHandler(InputEventHandler &inputEventHandler)
    : inputEventHandler(inputEventHandler) {
}

KeyboardEventHandler::~KeyboardEventHandler() {
}

bool KeyboardEventHandler::bindKey(int key, Function func) {
    int lowerKey = tolower(key);

    if (keyDownFuncMap.end() != keyDownFuncMap.find(lowerKey)) {
        std::cout << "Duplicate key '" << lowerKey << "' ignored." << std::endl;
        return false;
    } else {
        keyDownFuncMap[lowerKey].keyFunction = func;
        return true;
    }
}

bool KeyboardEventHandler::bindKey(int key, KeyState state, Function func) {
    int lowerKey = tolower(key);

    if (state == KEY_DOWN) {
        return bindKey(lowerKey, func);
    } else {
        if (keyUpFuncMap.end() != keyUpFuncMap.find(lowerKey)) {
            std::cout << "Duplicate key '" << lowerKey << "' ignored." << std::endl;
            return false;
        } else {
            keyUpFuncMap[lowerKey].keyFunction = func;
            return true;
        }
    }
}

bool KeyboardEventHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {
    int key = tolower(ea.getKey());

    switch (ea.getEventType()) {
        case osgGA::GUIEventAdapter::KEYDOWN: {
            bool newKeyDownEvent = false;
            auto itr = keyDownFuncMap.find(key);

            if (itr != keyDownFuncMap.end()) {
                if (itr->second.keyState == KEY_UP) {
                    itr->second.keyState = KEY_DOWN;
                    newKeyDownEvent = true;
                }

                if (newKeyDownEvent) {
                    (inputEventHandler.*(itr->second.keyFunction))();
                    newKeyDownEvent = false;
                }

                return true;
            }

            return false;
        }
        case osgGA::GUIEventAdapter::KEYUP: {
            bool newKeyUpEvent = false;
            auto itr = keyDownFuncMap.find(key);

            if (itr != keyDownFuncMap.end()) {
                itr->second.keyState = KEY_UP;
            }

            itr = keyUpFuncMap.find(key);

            if (itr != keyUpFuncMap.end()) {
                (inputEventHandler.*(itr->second.keyFunction))();
                return true;
            }

            return false;
        }
        default: {
            return false;
        }
    }
}

void KeyboardEventHandler::accept(osgGA::GUIEventHandlerVisitor &v) {
    v.visit(*this);
};

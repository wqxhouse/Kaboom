#include "KeyboardEventHandler.h"

#include <Core.h>

KeyboardEventHandler::KeyboardEventHandler(InputEventHandler &inputEventHandler)
        : inputEventHandler(inputEventHandler) {
}

bool KeyboardEventHandler::bindKey(int key, Function func) {
    int lowerKey = tolower(key);

    if (keyDownFuncMap.end() != keyDownFuncMap.find(lowerKey)) {
        std::cerr << "Duplicate key '" << lowerKey << "' ignored." << std::endl;
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
            std::cerr << "Duplicate key '" << lowerKey << "' ignored." << std::endl;
            return false;
        } else {
            keyUpFuncMap[lowerKey].keyFunction = func;
            return true;
        }
    }
}

bool KeyboardEventHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us) {
    if (!Core::isInGameMode()) {
        return false;
    }

    switch (ea.getEventType()) {
    case osgGA::GUIEventAdapter::KEYDOWN:
        return handleKeyDown(ea, us);
    case osgGA::GUIEventAdapter::KEYUP:
        return handleKeyUp(ea, us);
    }

    return false;
}

bool KeyboardEventHandler::handleKeyDown(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us) {
    int key = tolower(ea.getKey());
    auto itr = keyDownFuncMap.find(key);

    if (itr != keyDownFuncMap.end() && itr->second.keyState == KEY_UP) {
        itr->second.keyState = KEY_DOWN;
        (inputEventHandler.*(itr->second.keyFunction))();
        return true;
    }

    return false;
}

bool KeyboardEventHandler::handleKeyUp(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us) {
    int key = tolower(ea.getKey());
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

void KeyboardEventHandler::accept(osgGA::GUIEventHandlerVisitor &v) {
    v.visit(*this);
};

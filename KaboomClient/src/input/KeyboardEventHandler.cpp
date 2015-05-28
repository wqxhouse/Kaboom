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

bool KeyboardEventHandler::unbindKey(int key) {
	int lowerKey = tolower(key);

	if (keyDownFuncMap.end() != keyDownFuncMap.find(lowerKey)) {
		keyDownFuncMap.erase(lowerKey);
		return true;
	}
	else {
		std::cerr << "No such key '" << lowerKey << "' to be removed." << std::endl;
		return false;
	}
}

bool KeyboardEventHandler::unbindKey(int key, KeyState state) {
	int lowerKey = tolower(key);

	if (state == KEY_DOWN) {
		return unbindKey(lowerKey);
	}
	else {
		if (keyUpFuncMap.end() != keyUpFuncMap.find(lowerKey)) {
			keyUpFuncMap.erase(lowerKey);
			return true;
		}
		else {
			std::cerr << "No such key '" << lowerKey << "' to be removed." << std::endl;
			return false;
		}
	}
}

void KeyboardEventHandler::unbindKeyAll()
{
	keyUpFuncMap.clear();
	keyDownFuncMap.clear();
}

bool KeyboardEventHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us) {
	if (!Core::isInGameMode() && !Core::isInStartScreenMode() ) {
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
	else if (itr == keyDownFuncMap.end() && !Core::isInGameMode())
	{
		std::cout << "key pressed '" << key << "'." << std::endl;
		if (osgGA::GUIEventAdapter::KEY_BackSpace == key)
			inputEventHandler.removeCharacter();
		else
			inputEventHandler.typeCharacter(key);
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

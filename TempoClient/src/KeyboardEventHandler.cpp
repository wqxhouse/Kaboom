#include <iostream>

#include "Global.h"
#include "KeyboardEventHandler.h"

bool KeyboardEventHandler::registerKey(int key, Function func) {
	if (keyDownFuncMap.end() != keyDownFuncMap.find(key)) {
		std::cout << "Duplicate key '" << key << "' ignored." << std::endl;
		return false;
	}
	else {
		keyDownFuncMap[key].keyFunction = func;
		return true;
	}
}

bool KeyboardEventHandler::registerKey(int key, KeyState state, Function func) {
	if (state == KEY_DOWN) {
		return registerKey(key, func);
	}
	else {
		if (keyUpFuncMap.end() != keyUpFuncMap.find(key)) {
			std::cout << "Duplicate key '" << key << "' ignored." << std::endl;
			return false;
		}
		else {
			keyUpFuncMap[key].keyFunction = func;
			return true;
		}
	}
}

bool KeyboardEventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {
	KeyFunctionMap::iterator itr;
	bool newKeyDownEvent = false;
	bool newKeyUpEvent = false;

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
	default:
		return false;
	}
}

void KeyboardEventHandler::accept(osgGA::GUIEventHandlerVisitor& v) {
	v.visit(*this);
};

#include "MouseEventHandler.h"
#include <Core.h>
#include <osgViewer/GraphicsWindow>

MouseEventHandler::MouseEventHandler(InputEventHandler &inputEventHandler)
    : inputEventHandler(inputEventHandler) {
}

MouseEventHandler::~MouseEventHandler() {
}

bool MouseEventHandler::bindKey(int key, Function func) {
    if (keyDownFuncMap.end() != keyDownFuncMap.find(key)) {
        std::cout << "Duplicate key '" << key << "' ignored." << std::endl;
        return false;
    } else {
        keyDownFuncMap[key].keyFunction = func;
        return true;
    }
}

bool MouseEventHandler::bindKey(int key, KeyState state, Function func) {
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

bool MouseEventHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {
	if (!Core::isInGameMode()) return false;

    KeyFunctionMap::iterator itr;
    bool newKeyDownEvent = false;
    bool newKeyUpEvent = false;

    int centerX = (ea.getWindowX() + ea.getWindowWidth()) / 2;
    int centerY = (ea.getWindowY() + ea.getWindowHeight()) / 2;
    int x = ea.getX();
    int y = ea.getY();
    int dx = x - centerX;
    int dy = y - centerY;

    float dyaw = 0;
    float dpitch = 0;

    switch (ea.getEventType()) {
    case osgGA::GUIEventAdapter::KEYDOWN:
        itr = keyDownFuncMap.find(ea.getKey());

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
    case osgGA::GUIEventAdapter::KEYUP:
        itr = keyDownFuncMap.find(ea.getKey());

        if (itr != keyDownFuncMap.end()) {
            itr->second.keyState = KEY_UP;
        }

        itr = keyUpFuncMap.find(ea.getKey());

        if (itr != keyUpFuncMap.end()) {
            (inputEventHandler.*(itr->second.keyFunction))();
            return true;
        }

        return false;
	case osgGA::GUIEventAdapter::MOVE:
	{
		static int x = 0;
		if (dx != 0 || dy != 0) {
			dyaw = dx * Core::getLastFrameDuration() * 3; // TODO: add mouse sensitivity
			dpitch = dy * Core::getLastFrameDuration() * 3;

			inputEventHandler.onLook(dyaw, dpitch);
			aa.requestWarpPointer(centerX, centerY);
			return true;
		}

		return true;
	}
    default:
        return false;
    }

}

void MouseEventHandler::accept(osgGA::GUIEventHandlerVisitor &v) {
    v.visit(*this);
};

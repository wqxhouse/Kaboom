#include "MouseEventHandler.h"
#include <Core.h>
#include <osgViewer/GraphicsWindow>

MouseEventHandler::MouseEventHandler(InputEventHandler &inputEventHandler)
        : inputEventHandler(inputEventHandler) {
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
    if (!Core::isInGameMode()) {
        return false;
    }

    switch (ea.getEventType()) {
        case osgGA::GUIEventAdapter::PUSH: {
            bool newMouseDownEvent = false;
            auto itr = keyDownFuncMap.find(ea.getButton());

            if (itr != keyDownFuncMap.end()) {
                if (itr->second.keyState == KEY_UP) {
                    itr->second.keyState = KEY_DOWN;
                    newMouseDownEvent = true;
                }

                if (newMouseDownEvent) {
                    (inputEventHandler.*(itr->second.keyFunction))();
                    newMouseDownEvent = false;
                }

                return true;
            }

            return false;
        }
        case osgGA::GUIEventAdapter::RELEASE: {
            bool newMouseUpEvent = false;
            auto itr = keyDownFuncMap.find(ea.getButton());

            if (itr != keyDownFuncMap.end()) {
                itr->second.keyState = KEY_UP;
            }

            itr = keyUpFuncMap.find(ea.getButton());

            if (itr != keyUpFuncMap.end()) {
                (inputEventHandler.*(itr->second.keyFunction))();
                return true;
            }

            return false;
        }
        case osgGA::GUIEventAdapter::DRAG:
        case osgGA::GUIEventAdapter::MOVE: {
            int centerX = (ea.getWindowX() + ea.getWindowWidth()) / 2;
            int centerY = (ea.getWindowY() + ea.getWindowHeight()) / 2;
            int dx = ea.getX() - centerX;
            int dy = ea.getY() - centerY;

            if (dx != 0 || dy != 0) {
                float dyaw = dx * Core::getLastFrameDuration() * 3; // TODO: add mouse sensitivity
                float dpitch = dy * Core::getLastFrameDuration() * 3;

                inputEventHandler.onLook(dyaw, dpitch);
                aa.requestWarpPointer(centerX, centerY);
                return true;
            }

            return true;
        }
        default: {
            return false;
        }
    }

}

void MouseEventHandler::accept(osgGA::GUIEventHandlerVisitor &v) {
    v.visit(*this);
};

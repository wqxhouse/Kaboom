#include "MouseEventHandler.h"

#include <osgViewer/GraphicsWindow>

#include <Core.h>

MouseEventHandler::MouseEventHandler(InputEventHandler &inputEventHandler)
        : inputEventHandler(inputEventHandler) {
}

bool MouseEventHandler::bindKey(int key, Function func) {
    if (keyDownFuncMap.end() != keyDownFuncMap.find(key)) {
        std::cerr << "Duplicate key '" << key << "' ignored." << std::endl;
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
            std::cerr << "Duplicate key '" << key << "' ignored." << std::endl;
            return false;
        } else {
            keyUpFuncMap[key].keyFunction = func;
            return true;
        }
    }
}

bool MouseEventHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us) {
    if (!Core::isInGameMode() || Core::isInDeath()) {
		handleMouseUp(ea, us);
        return false;
    }

    switch (ea.getEventType()) {
    case osgGA::GUIEventAdapter::PUSH:
        return handleMouseDown(ea, us);
    case osgGA::GUIEventAdapter::RELEASE:
        return handleMouseUp(ea, us);
    case osgGA::GUIEventAdapter::DRAG:
    case osgGA::GUIEventAdapter::MOVE:
        return handleMouseMove(ea, us);
    }

    return false;
}

bool MouseEventHandler::handleMouseDown(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us) {
    auto itr = keyDownFuncMap.find(ea.getButton());

    if (itr != keyDownFuncMap.end() && itr->second.keyState == KEY_UP) {
        itr->second.keyState = KEY_DOWN;
        (inputEventHandler.*(itr->second.keyFunction))();

        return true;
    }

    return false;
}

bool MouseEventHandler::handleMouseUp(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us) {
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

bool MouseEventHandler::handleMouseMove(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us) {
    int centerX = (ea.getWindowX() + ea.getWindowWidth()) / 2;
    int centerY = (ea.getWindowY() + ea.getWindowHeight()) / 2;
    int dx = ea.getX() - centerX;
    int dy = ea.getY() - centerY;

    if ( (dx > 1 || dx < -1) || (dy > 1 || dy <-1)) {
        float dyaw = dx * Core::getLastFrameDuration() * 3; // TODO: add mouse sensitivity
        float dpitch = dy * Core::getLastFrameDuration() * 3;

        inputEventHandler.onLook(dyaw, dpitch);
        us.requestWarpPointer(centerX, centerY);
        return true;
    }

    return true;
}

void MouseEventHandler::accept(osgGA::GUIEventHandlerVisitor &v) {
    v.visit(*this);
};

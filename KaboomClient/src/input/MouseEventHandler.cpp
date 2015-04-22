#include "MouseEventHandler.h"

#include "InputEventHandler.h"

MouseEventHandler::MouseEventHandler(InputEventHandler &inputEventHandler)
    : inputEventHandler(inputEventHandler) {
}

MouseEventHandler::~MouseEventHandler() {
}

bool MouseEventHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {
    int centerX = (ea.getWindowX() + ea.getWindowWidth()) / 2;
    int centerY = (ea.getWindowY() + ea.getWindowHeight()) / 2;
    int x = ea.getX();
    int y = ea.getY();
    int dx = x - centerX;
    int dy = y - centerY;

    float yaw = 0;
    float pitch = 0;

    switch (ea.getEventType()) {
    case osgGA::GUIEventAdapter::KEYDOWN:
        return false;
    case osgGA::GUIEventAdapter::KEYUP:
        return false;
    case osgGA::GUIEventAdapter::MOVE:
        if (dx != 0 || dy != 0) {
            inputEventHandler.onLook(yaw, pitch);
            aa.requestWarpPointer(centerX, centerY);

            return true;
        }

        return false;
    default:
        return false;
    }

}

void MouseEventHandler::accept(osgGA::GUIEventHandlerVisitor &v) {
    v.visit(*this);
};

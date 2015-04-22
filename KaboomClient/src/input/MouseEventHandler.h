#pragma once

#include <osgGA/GUIEventHandler>

class InputEventHandler;

class MouseEventHandler : public osgGA::GUIEventHandler {
public:
    MouseEventHandler(InputEventHandler &inputEventHandler);
    ~MouseEventHandler();

    virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    virtual void accept(osgGA::GUIEventHandlerVisitor &v);

private:
    InputEventHandler &inputEventHandler;
};

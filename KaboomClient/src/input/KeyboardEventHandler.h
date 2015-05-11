#pragma once

#include <osgGA/GUIEventHandler>

#include "InputEventHandler.h"

class KeyboardEventHandler : public osgGA::GUIEventHandler {
public:
    typedef void(InputEventHandler::*Function) ();

    enum KeyState {
        KEY_UP,
        KEY_DOWN
    };

    struct FunctionState {
        FunctionState() {
            keyState = KEY_UP;
            keyFunction = NULL;
        }

        Function keyFunction;
        KeyState keyState;
    };

    typedef std::map<int, FunctionState> KeyFunctionMap;

    KeyboardEventHandler(InputEventHandler &inputEventHandler);

    bool bindKey(int key, Function func);
    bool bindKey(int key, KeyState state, Function func);

    virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us);
    bool handleKeyDown(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us);
    bool handleKeyUp(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us);

    virtual void accept(osgGA::GUIEventHandlerVisitor &v);

private:
    InputEventHandler &inputEventHandler;

    KeyFunctionMap keyDownFuncMap;
    KeyFunctionMap keyUpFuncMap;
};

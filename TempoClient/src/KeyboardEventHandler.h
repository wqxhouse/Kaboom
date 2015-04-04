#pragma once

#include <Windows.h>

#include <osgGA/GUIEventHandler>

class KeyboardEventHandler : public osgGA::GUIEventHandler {
public:
	typedef void(*Function) ();

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

	bool registerKey(int key, Function func);
	bool registerKey(int key, KeyState state, Function func);

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	virtual void accept(osgGA::GUIEventHandlerVisitor& v);

protected:
	KeyFunctionMap keyDownFuncMap;
	KeyFunctionMap keyUpFuncMap;
};

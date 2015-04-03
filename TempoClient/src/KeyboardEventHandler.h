#pragma once

#include <Windows.h>

#include <osgGA/GUIEventHandler>

class KeyboardEventHandler : public osgGA::GUIEventHandler {
public:
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	virtual void accept(osgGA::GUIEventHandlerVisitor& v);
};

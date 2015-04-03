#include <iostream>

#include "Global.h"
#include "KeyboardEventHandler.h"

bool KeyboardEventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {
	switch (ea.getEventType()) {
	case osgGA::GUIEventAdapter::KEYDOWN:
		switch (ea.getKey()) {
		case 'w':
			position.y() += 0.05;
			return false;
		case 's':
			position.y() -= 0.05;
			return false;
		case 'a':
			position.x() -= 0.05;
			return false;
		case 'd':
			position.x() += 0.05;
			return false;
		default:
			return false;
		}
	default:
		return false;
	}
}

void KeyboardEventHandler::accept(osgGA::GUIEventHandlerVisitor& v) {
	v.visit(*this);
};

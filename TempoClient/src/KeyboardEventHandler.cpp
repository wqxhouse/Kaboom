#include <iostream>

#include "KeyboardEventHandler.h"

bool KeyboardEventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {
	switch (ea.getEventType()) {
	case osgGA::GUIEventAdapter::KEYDOWN:
		switch (ea.getKey()) {
		case 'w':
			std::cout << "w key pressed" << std::endl;
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

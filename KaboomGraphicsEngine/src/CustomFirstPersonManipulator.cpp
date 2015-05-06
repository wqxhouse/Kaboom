//
//  CustomFirstPersonManipulator.cpp
//  vrphysics
//
//  Created by Robin Wu on 3/5/15.
//  Copyright (c) 2015 WSH. All rights reserved.
//
#include "CustomFirstPersonManipulator.h"
#include <Core.h>

using namespace osgGA;

CustomFirstPersonManipulator::CustomFirstPersonManipulator(int flag)
	: osgGA::FirstPersonManipulator(flag)
{

}

CustomFirstPersonManipulator::~CustomFirstPersonManipulator()
{

}

/** Handles events. Returns true if handled, false otherwise.*/
bool CustomFirstPersonManipulator::handle(const GUIEventAdapter& ea, GUIActionAdapter& us)
{
	// disable scrolling
	if (ea.getScrollingMotion() == GUIEventAdapter::SCROLL_UP)
	{
		return true;
	}
	else if (ea.getScrollingMotion() == GUIEventAdapter::SCROLL_DOWN)
	{
		return true;
	}

	bool res = StandardManipulator::handle(ea, us);
	flushKeyBuffer();
	return res;
}

void CustomFirstPersonManipulator::flushKeyBuffer()
{
	double spf = Core::getLastFrameDuration();
	for (int i = 0; i < _keyBuffer.size(); i++)
	{
		int key = _keyBuffer[i];
		if (key == 0)
		{
			moveForward(10.0 * spf);
		}
		else if (key == 1)
		{
			moveForward(-10.0 * spf);
		}
		else if (key == 2)
		{
			moveRight(-10.0 * spf);
		}
		else if (key == 3)
		{
			moveRight(10.0 * spf);
		}
	}
	_keyBuffer.clear();
}

bool CustomFirstPersonManipulator::handleKeyUp(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
	return true;
}

bool CustomFirstPersonManipulator::handleKeyDown(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
	if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Space)
	{
		flushMouseEventStack();
		_thrown = false;
		home(ea, us);
		return true;
	}
	else if (ea.getKey() == 'w')
	{
		_keyBuffer.push_back(0);
	}
	else if (ea.getKey() == 's')
	{
		_keyBuffer.push_back(1);
	}
	else if (ea.getKey() == 'a')
	{
		_keyBuffer.push_back(2);
	}
	else if (ea.getKey() == 'd')
	{
		_keyBuffer.push_back(3);
	}

	return false;
}
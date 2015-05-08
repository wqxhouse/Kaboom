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

bool CustomFirstPersonManipulator::handleFrame(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us)
{
	double spf = Core::getLastFrameDuration();

	if (_movingForward)
	{
		moveForward(10.0 * spf);
	}
	else if (_movingBackward)
	{
		moveForward(-10.0 * spf);
	}

	if (_movingLeft)
	{
		moveRight(-10.0 * spf);
	}
	else if (_movingRight)
	{
		moveRight(10.0 * spf);
	}

	return StandardManipulator::handleFrame(ea, us);
}

/** Handles events. Returns true if handled, false otherwise.*/
bool CustomFirstPersonManipulator::handleKeyDown(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us)
{
	int key = tolower(ea.getKey());

	switch (key)
	{
	case osgGA::GUIEventAdapter::KEY_W:
		_movingForward = true;
		return true;
	case osgGA::GUIEventAdapter::KEY_S:
		_movingBackward = true;
		return true;
	case osgGA::GUIEventAdapter::KEY_A:
		_movingLeft = true;
		return true;
	case osgGA::GUIEventAdapter::KEY_D:
		_movingRight = true;
		return true;
	case osgGA::GUIEventAdapter::KEY_Space:
		flushMouseEventStack();
		_thrown = false;
		home(ea, us);
		return true;
	}

	return StandardManipulator::handleKeyDown(ea, us);
}

/** Handles events. Returns true if handled, false otherwise.*/
bool CustomFirstPersonManipulator::handleKeyUp(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us)
{
	int key = tolower(ea.getKey());

	switch (key)
	{
		case osgGA::GUIEventAdapter::KEY_W:
			_movingForward = false;
			return true;
		case osgGA::GUIEventAdapter::KEY_S:
			_movingBackward = false;
			return true;
		case osgGA::GUIEventAdapter::KEY_A:
			_movingLeft = false;
			return true;
		case osgGA::GUIEventAdapter::KEY_D:
			_movingRight = false;
			return true;
	}

	return StandardManipulator::handleKeyUp(ea, us);
}

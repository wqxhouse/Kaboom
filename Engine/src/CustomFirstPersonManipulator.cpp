//
//  CustomFirstPersonManipulator.cpp
//  vrphysics
//
//  Created by Robin Wu on 3/5/15.
//  Copyright (c) 2015 WSH. All rights reserved.
//
#include "CustomFirstPersonManipulator.h"

CustomFirstPersonManipulator::CustomFirstPersonManipulator(int flag)
	: osgGA::FirstPersonManipulator(flag)
{

}

CustomFirstPersonManipulator::~CustomFirstPersonManipulator()
{

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
	if (ea.getKey() == 'w')
	{
		osg::Vec3d eye;
		osg::Vec3d center;
		osg::Vec3d up;
		moveForward(5.0);
		return true;
	}
	if (ea.getKey() == 's')
	{
		moveForward(-5.0);
		return true;
	}
	if (ea.getKey() == 'a')
	{
		moveRight(-5.0);
		return true;
	}
	if (ea.getKey() == 'd')
	{
		moveRight(5.0);
		return true;
	}
	return false;
}
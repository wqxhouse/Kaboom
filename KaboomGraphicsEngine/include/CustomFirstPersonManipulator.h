//
//  CustomFirstPersonManipulator.h
//  vrphysics
//
//  Created by Robin Wu on 3/5/15.
//  Copyright (c) 2015 WSH. All rights reserved.
//

#ifndef __vrphysics__CustomFirstPersonManipulator__
#define __vrphysics__CustomFirstPersonManipulator__

#include <stdio.h>
#include <vector>
#include <osgGA/FirstPersonManipulator>

class CustomFirstPersonManipulator : public osgGA::FirstPersonManipulator
{
public:
	CustomFirstPersonManipulator(int flags = DEFAULT_SETTINGS);
	virtual ~CustomFirstPersonManipulator();

protected:
	virtual bool handleKeyDown(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);
	virtual bool handleKeyUp(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);
	void flushKeyBuffer();
	std::vector<int> _keyBuffer;
};

#endif /* defined(__vrphysics__CustomFirstPersonManipulator__) */
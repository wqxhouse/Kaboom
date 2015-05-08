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

	void setWalkingSpeed(float metersPerSec);
	float getWalkingSpeed();

protected:
	virtual bool handleFrame(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us);
	virtual bool handleKeyDown(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us);
	virtual bool handleKeyUp(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us);

	bool _movingForward;
	bool _movingBackward;
	bool _movingLeft;
	bool _movingRight;

	float _metersPerSec;
};

#endif /* defined(__vrphysics__CustomFirstPersonManipulator__) */
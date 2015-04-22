#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <SDKDDKVer.h>
#include <cstdio>

#include <ostream>
#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <iostream>

#include <WS2tcpip.h>
#include <WinSock2.h>

#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osgGA/GUIEventHandler>

#include <ConfigSettings.h>
#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <SDKDDKVer.h>
#include <stdio.h>

#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <Windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <osg/PolygonMode>
#include <osg/PolygonOffset>
#include <osgbCollision/CollisionShapes.h>
#include <osgbCollision/GLDebugDrawer.h>
#include <osgbDynamics/MotionState.h>
#include <osgDB/ReadFile>
#include <osgDB/XmlParser>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

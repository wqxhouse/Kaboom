#pragma once
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <SDKDDKVer.h>

#include <assert.h>

#include <osg/Group>
#include <osg/Scissor>
#include <osg/Texture2D>
#include <osg/Geometry>
#include <osg/BlendFunc>
#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>

#include <Rocket/Core/Core.h>
#include <Rocket/Debugger/Debugger.h>
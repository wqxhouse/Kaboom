#pragma once
#include <osg/Texture2D>
#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgUtil/LineSegmentIntersector>
#include <osgText/Text>
#include <osgViewer/Viewer>

#include "EffectCompositor.h"

class CompositorAnalysis : public osgGA::GUIEventHandler
{
public:
	struct Connection
	{
		std::vector<osg::Vec3> starts;
		std::vector<osg::Vec3> ends;
	};

	CompositorAnalysis(osg::Camera* cam, osgFX::EffectCompositor* c, int numBuffers = 3);
	void toggleHelper();
	void applyEffectGraph(osgFX::EffectCompositor* c, float x, float y, float w, float h);
	void applyUniformList(osgFX::EffectCompositor* c, float x, float y, float w, float h);
	unsigned int applyBufferData(osgFX::EffectCompositor* c, int startIndex = 0);
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

protected:
	std::map<osg::Uniform*, osgText::Text*> _uniformTextMap;
	std::vector<osg::Geometry*> _bufferQuads;
	std::vector<osgText::Text*> _bufferTexts;
	osg::observer_ptr<osg::Geode> _bufferDisplay;
	osg::observer_ptr<osg::Geode> _uniformDisplay;
	osg::observer_ptr<osg::Geode> _graphDisplay;
	osg::observer_ptr<osg::Camera> _hudCamera;
	osg::observer_ptr<osgFX::EffectCompositor> _compositor;
	unsigned int _numBuffers;
	unsigned int _startIndex;
};
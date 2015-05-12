#pragma once
#include "Visualizer.h"
#include <vector>

#include <osg/Group>
#include <osg/Node>
#include <osg/Billboard>

#include "Light.h"

class LightVisualizer : public Visualizer
{
public:
	LightVisualizer();
	void addLight(Light *light);
	void removeLight(Light *light);

	Light *getLightFromDrawable(osg::Drawable *lightDrawable);

private:
	std::vector<Light *> _lightPtrs;

	void init();
	osg::Geometry *createPointLightTexturedQuad();

	osg::ref_ptr<osg::Texture2D> _pointLightTex;
	osg::ref_ptr<osg::Billboard> _lightBillBoard;
	osg::ref_ptr<osg::Vec4Array> _quadColorArr;
};

class LightVisualizerCallback : public osg::NodeCallback
{
public:
	LightVisualizerCallback(std::vector<Light *> &lightPtrs);
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

private:
	std::vector<Light *> & _lightPtrsRef;
};

class LightVisualizerWrapper : public osg::Referenced
{
public:
	LightVisualizerWrapper(LightVisualizer *visualizer)
		: _lightVisualizer(visualizer) {};

	inline LightVisualizer *getLightVisualizer()
	{
		return _lightVisualizer;
	}

private:
	LightVisualizer *_lightVisualizer;
};

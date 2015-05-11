#include "LightVisualizer.h"
#include <osg/Shape>

LightVisualizer::LightVisualizer()
{
	makeLightGeode();
}

LightVisualizer::LightVisualizer(const osg::Vec3 &pos)
: LightVisualizer()
{
	setPosition(pos);
}

void LightVisualizer::makeLightGeode()
{
	float radius = 0.5f;
	//osg::Vec4 color = osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f);

	osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere;
	sphere->set(osg::Vec3(), radius);

	osg::ref_ptr<osg::ShapeDrawable> _sd = new osg::ShapeDrawable;
	_sd->setShape(sphere);
	//_sd->setColor(color);

	osg::ref_ptr<osg::Geode> _geo = new osg::Geode;
	_geo->addDrawable(_sd);

	_matTrans->addChild(_geo);
}
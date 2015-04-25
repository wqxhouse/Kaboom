#include "AxisVisualizer.h"
#include <osg/Shape>

AxisVisualizer::AxisVisualizer()
{
	_cam = new osg::Camera;
	_matTrans->addChild(_cam); // matTrans should not change cam

	makeAxisGeode();
}

void AxisVisualizer::makeAxisGeode()
{
	osg::ref_ptr<osg::Cylinder> cylinder = new osg::Cylinder;
	cylinder->set(osg::Vec3(), 0.05, 1);

	osg::ref_ptr<osg::ShapeDrawable> _xsd = new osg::ShapeDrawable;
	osg::ref_ptr<osg::ShapeDrawable> _ysd = new osg::ShapeDrawable;
	osg::ref_ptr<osg::ShapeDrawable> _zsd = new osg::ShapeDrawable;
	_xsd->setShape(cylinder);
	_ysd->setShape(cylinder);
	_zsd->setShape(cylinder);
	_xsd->setColor(osg::Vec4(1, 0, 0, 1));
	_ysd->setColor(osg::Vec4(0, 1, 0, 1));
	_zsd->setColor(osg::Vec4(0, 0, 1, 1));

	osg::ref_ptr<osg::Geode> _xgeo = new osg::Geode;
	osg::ref_ptr<osg::Geode> _ygeo = new osg::Geode;
	osg::ref_ptr<osg::Geode> _zgeo = new osg::Geode;

	_xgeo->addDrawable(_xsd);
	_ygeo->addDrawable(_ysd);
	_zgeo->addDrawable(_zsd);

	osg::ref_ptr<osg::MatrixTransform> _x = new osg::MatrixTransform;
	osg::ref_ptr<osg::MatrixTransform> _y = new osg::MatrixTransform;
	osg::ref_ptr<osg::MatrixTransform> _z = new osg::MatrixTransform;

	osg::Matrix matX;
	matX.makeRotate(osg::DegreesToRadians(90.0), osg::Vec3(0, 1, 0));
	matX *= matX.translate(osg::Vec3(0.5, 0, 0));

	osg::Matrix matY;
	matY.makeRotate(osg::DegreesToRadians(90.0), osg::Vec3(1, 0, 0));
	matY *= matY.translate(osg::Vec3(0, 0.5, 0));

	osg::Matrix matZ;
	matZ.makeTranslate(osg::Vec3(0, 0, 0.5));

	_x->setMatrix(matX);
	_y->setMatrix(matY);
	_z->setMatrix(matZ);

	_x->addChild(_xgeo);
	_y->addChild(_ygeo);
	_z->addChild(_zgeo);

	_cam->addChild(_x);
	_cam->addChild(_y);
	_cam->addChild(_z);
}

void AxisVisualizer::configCamera()
{
	_cam->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
}
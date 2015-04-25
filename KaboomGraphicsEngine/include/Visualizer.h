#pragma once
#include <osg/Vec3>

class Visualizer
{
public:	
	Visualizer();
	void setPosition(const osg::Vec3 &pos);

	inline osg::ref_ptr<osg::MatrixTransform> getRoot()
	{
		return _matTrans;
	}
	
protected:
	osg::ref_ptr<osg::MatrixTransform> _matTrans;
};
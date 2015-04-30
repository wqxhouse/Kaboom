#include "Visualizer.h"
#include <osg/Depth>

Visualizer::Visualizer()
{
	_matTrans = new osg::MatrixTransform;
	_matTrans->setNodeMask(0x4); // same for manipulator

	osg::ref_ptr<osg::Depth> depth = new osg::Depth;
	depth->setWriteMask(false);
	_matTrans->getOrCreateStateSet()->setAttributeAndModes(depth);
	_matTrans->setCullingActive(false);
}

void Visualizer::setPosition(const osg::Vec3 &pos)
{
	osg::Matrix trans;
	trans.makeTranslate(pos);
	_matTrans->setMatrix(trans);
}

#include "GeometryObjectManipulator.h"
#include <osg/ComputeBoundsVisitor>
#include "Core.h"

DraggerUpdateCallback GeometryObjectManipulator::_draggerCB = DraggerUpdateCallback(NULL);

void GeometryObjectManipulator::initWithRootNode(osg::Group *root)
{
	_rootNode = root;
	_currType = NONE;
	
	if (_depth == NULL)
	{
		_depth = new osg::Depth;
		_depth->setFunction(osg::Depth::LESS);
		_depth->setWriteMask(false);
	}
}

enum ManipulatorType GeometryObjectManipulator::getCurrentManipulatorType()
{
	return _currType;
}

void GeometryObjectManipulator::detachManipulator()
{
	if (_rootNode.valid() && _currNode.valid() && _dragger.valid())
	{
		_dragger->removeTransformUpdating(_currNode.get());
		_dragger->setHandleEvents(false);
		_rootNode->removeChild(_dragger.get());
		_dragger = NULL;

		// TODO: test if this crash the program
		_currNode = NULL;
		_currType = NONE;
	}
}

void GeometryObjectManipulator::changeCurrentManipulatorType(enum ManipulatorType type)
{
	assignManipulatorToGeometryTransformNode(_currNode.get(), type);
}


void GeometryObjectManipulator::changeCurrentNode(osg::MatrixTransform *node)
{
	if (_currType != NONE)
	{
		assignManipulatorToGeometryTransformNode(node, _currType);
	}
	else
	{
		assignManipulatorToGeometryTransformNode(node, TabBoxDragger);
		_currType = TabBoxDragger;
	}
}

void GeometryObjectManipulator::assignManipulatorToGeometryTransformNode
	(osg::MatrixTransform *node, enum ManipulatorType type)
{
	if (Core::isInGameMode()) return;

	if (node == NULL || !_rootNode.valid())
	{
		return;
	}

	// unassign prev node dragger
	if (_currNode.valid()  && _dragger.valid())
	{
		// _currNode->removeChild(_dragger.get());
		_dragger->removeTransformUpdating(_currNode.get());
		_dragger->setHandleEvents(false);
		_rootNode->removeChild(_dragger.get());
		_dragger = NULL;
	}

	_currNode = node;
	
	switch (type)
	{
	case TrackballDragger:
		if (_trackBallDragger == NULL)
		{
			_trackBallDragger = new osgManipulator::TrackballDragger;
			_trackBallDragger->setupDefaultGeometry();
			_trackBallDragger->getOrCreateStateSet()->setAttributeAndModes(_depth, osg::StateAttribute::ON);
		}
		_trackBallDragger->setNodeMask(0x4);

		_dragger = _trackBallDragger.get();

		break;
	case TranslateAxisDragger:
		if (_translateAxisDragger == NULL)
		{
			_translateAxisDragger = new osgManipulator::TranslateAxisDragger;
			_translateAxisDragger->setupDefaultGeometry();
			_translateAxisDragger->getOrCreateStateSet()->setAttributeAndModes(_depth, osg::StateAttribute::ON);
		}

		_translateAxisDragger->setNodeMask(0x4);
		_dragger = _translateAxisDragger.get();

		break;
	case TabBoxDragger:
	{
		if (_tabBoxDragger == NULL)
		{
			_tabBoxDragger = new osgManipulator::TabBoxDragger;
			_tabBoxDragger->setupDefaultGeometry();
			_tabBoxDragger->getOrCreateStateSet()->setAttributeAndModes(_depth, osg::StateAttribute::ON);
		}

		_tabBoxDragger->setNodeMask(0x4);

		osg::ComputeBoundsVisitor bound;
		_currNode->accept(bound);

		osg::BoundingBox bbox = bound.getBoundingBox();
		float xscale = (bbox.xMax() - bbox.xMin());
		float yscale = (bbox.yMax() - bbox.yMin());
		float zscale = (bbox.zMax() - bbox.zMin());

		osg::Vec3f center = bbox.center();
		_tabBoxDragger->setMatrix(osg::Matrix::scale(xscale, yscale, zscale) *
			osg::Matrix::translate(center));

		_dragger = _tabBoxDragger.get();
		break;
	}
	default:
		OSG_WARN << "GeometryObjectManipulator: Dragger not implemented" << std::endl;
	}

	if (_dragger.valid())
	{
		_currType = type;
		float scale = _currNode->getBound().radius();
		if (type != TabBoxDragger)
		{
			if (type == TranslateAxisDragger) scale *= 1.6;
			_dragger->setMatrix(osg::Matrix::scale(scale, scale, scale) *
				osg::Matrix::translate(_currNode->getBound().center()));
		}

		_dragger->addTransformUpdating(_currNode.get());

		_draggerCB.setNode(_currNode.get());
		_dragger->addDraggerCallback(&_draggerCB);

		_dragger->setHandleEvents(true);
		_rootNode->addChild(_dragger.get());
	}
}

osg::ref_ptr<osg::MatrixTransform> GeometryObjectManipulator::getCurrNode()
{
	return _currNode;
}

void GeometryObjectManipulator::updateBoundingBox() 
{
	if (!_currNode.valid()) return;
	if (!_dragger.valid()) return;

	float scale = _currNode->getBound().radius();
	if (_currType != TabBoxDragger)
	{
		if (_currType == TranslateAxisDragger) scale *= 1.6;
		_dragger->setMatrix(osg::Matrix::scale(scale, scale, scale) *
			osg::Matrix::translate(_currNode->getBound().center()));
	}
	else {
		osg::ComputeBoundsVisitor bound;
		_currNode->accept(bound);

		osg::BoundingBox bbox = bound.getBoundingBox();
		float xscale = (bbox.xMax() - bbox.xMin());
		float yscale = (bbox.yMax() - bbox.yMin());
		float zscale = (bbox.zMax() - bbox.zMin());

		osg::Vec3f center = bbox.center();
		_dragger->setMatrix(osg::Matrix::scale(xscale, yscale, zscale) *
			osg::Matrix::translate(center));
	}
}

bool GeometryObjectManipulator::setVisible(bool tf)
{
	if (_dragger.valid())
	{
		if (tf)
		{
			_dragger->setNodeMask(0x4);
			return true;
		}
		else
		{
			_dragger->setNodeMask(0x0);
			return true;
		}
	}
	return false;
}

bool GeometryObjectManipulator::isVisible()
{
	if (!_dragger.valid()) return false;
	return _dragger->getNodeMask() == 0x4 ? true : false;
}

osg::ref_ptr<osgManipulator::TrackballDragger> GeometryObjectManipulator::_trackBallDragger = NULL;
osg::ref_ptr<osgManipulator::TranslateAxisDragger> GeometryObjectManipulator::_translateAxisDragger = NULL;
osg::ref_ptr<osgManipulator::TabBoxDragger> GeometryObjectManipulator::_tabBoxDragger = NULL;
osg::observer_ptr<osg::MatrixTransform> GeometryObjectManipulator::_currNode = NULL;
osg::observer_ptr<osgManipulator::Dragger> GeometryObjectManipulator::_dragger = NULL;
osg::observer_ptr<osg::Group> GeometryObjectManipulator::_rootNode = NULL;
osg::ref_ptr<osg::Depth> GeometryObjectManipulator::_depth = NULL;
enum ManipulatorType GeometryObjectManipulator::_currType;
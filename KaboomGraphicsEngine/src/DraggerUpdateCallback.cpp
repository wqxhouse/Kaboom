#include "DraggerUpdateCallback.h"

#include "Core.h"
#include "TwGUIManager.h"
#include "GeometryObjectManager.h"
#include "GeometryObjectManipulator.h"

DraggerUpdateCallback::DraggerUpdateCallback(
	 osg::MatrixTransform *transform, int handleCommandMask)
: osgManipulator::DraggerTransformCallback(transform, handleCommandMask)
{
	setNode(transform);
}

void DraggerUpdateCallback::setNode(osg::MatrixTransform *transform) {
	_matTrans = transform;

	if (transform != NULL) {
		if (GeometryObjectManipulator::isAttchingLight()) return;

		std::string nodeName = transform->getName();
		std::string prefix("Transform_");

		_geomName = nodeName.substr(prefix.length());
	}
}

bool DraggerUpdateCallback::receive(const osgManipulator::MotionCommand & cmd)
{
	// TODO: make light undo/redo
	if (GeometryObjectManipulator::isAttchingLight())
	{
		if (cmd.getStage() == osgManipulator::MotionCommand::MOVE)
		{
			// TODO: probably bad design. This will only work assuming
			// the matrix position is handled by the first draggercallback in 
			// the callback list. This means the current dragger update must
			// not be the first callback. 
			osg::Vec3 new_position = osg::Vec3() * _matTrans->getMatrix();
			Light *light = GeometryObjectManipulator::getAttachedLight();
			light->setPosition(new_position);
		}
	}
	else
	{
		GeometryObject* geomObj = Core::getWorldRef().getGeometryManager()->getGeometryObject(_geomName);
		if (geomObj == NULL) return false;

		switch (cmd.getStage())
		{
		case osgManipulator::MotionCommand::START:
			if (geomObj != NULL) {
				TwGUIManager::addGeomToUndo(geomObj);
			}
			break;
		case osgManipulator::MotionCommand::FINISH:
			if (geomObj != NULL) {
				TwGUIManager::_currChange = TwGUIManager::makeModelMatrix(geomObj);
			}
			break;
		}
	}

	return true;
}
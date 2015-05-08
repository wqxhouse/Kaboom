#include "DraggerUpdateCallback.h"

#include "Core.h"
#include "TwGUIManager.h"
#include "GeometryObjectManager.h"

DraggerUpdateCallback::DraggerUpdateCallback(
	 osg::MatrixTransform *transform, int handleCommandMask)
: osgManipulator::DraggerTransformCallback(transform, handleCommandMask)
{
	setNode(transform);
}

void DraggerUpdateCallback::setNode(osg::MatrixTransform *transform) {
	if (transform != NULL) {
		std::string nodeName = transform->getName();
		std::string prefix("Transform_");

		_geomName = nodeName.substr(prefix.length());
	}
}

bool DraggerUpdateCallback::receive(const osgManipulator::MotionCommand & cmd)
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

	return true;
}
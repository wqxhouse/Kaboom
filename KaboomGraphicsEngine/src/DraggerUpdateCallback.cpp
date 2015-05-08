#include "DraggerUpdateCallback.h"

#include "Core.h"
#include "TwGUIManager.h"

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

		std::string realName = nodeName.substr(prefix.length());

		if (realName != "") {
			_geomObj = Core::getWorldRef().getGeometryManager()->
				getGeometryObject(realName);
		}
	}
}

bool DraggerUpdateCallback::receive(const osgManipulator::MotionCommand & cmd)
{
	switch (cmd.getStage())
	{
	case osgManipulator::MotionCommand::START:
		if (_geomObj != NULL) {
			TwGUIManager::addGeomToUndo(_geomObj);
		}
		break;
	case osgManipulator::MotionCommand::FINISH:
		if (_geomObj != NULL) {
			TwGUIManager::_currChange = TwGUIManager::makeModelMatrix(_geomObj);
		}
		break;
	}

	return true;
}
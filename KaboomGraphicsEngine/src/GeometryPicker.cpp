#include "GeometryPicker.h"
#include "EffectCompositor.h"
#include "GeometryObjectManipulator.h"
#include <Core.h>

bool GeometryPicker::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	if (Core::isInGameMode()) return false;
	osgViewer::View* view = dynamic_cast<osgViewer::View*> (&aa);
	if (NULL == view)
	{
		return false;
	}

	switch (ea.getEventType())
	{
	case osgGA::GUIEventAdapter::PUSH:
	{
		_mX = ea.getX();
		_mY = ea.getY();
	}
		break;

	case osgGA::GUIEventAdapter::RELEASE:
	{
		if (ea.getX() == _mX && ea.getY() == _mY)
		{
			if (Core::isCamLocked())
			{
				pick(ea, aa);
			}
		}
	}
		break;

	case osgGA::GUIEventAdapter::KEYDOWN:
		if (Core::isCamLocked())
		{
			if (ea.getKey() == 'g')
			{
				GeometryObjectManipulator::changeCurrentManipulatorType(TranslateAxisDragger);
			}
			else if (ea.getKey() == 'r')
			{
				GeometryObjectManipulator::changeCurrentManipulatorType(TrackballDragger);
			}
			else if (ea.getKey() == 's')
			{
				GeometryObjectManipulator::changeCurrentManipulatorType(TabBoxDragger);
			}
		}
		break;

	default:
		break;
	}

	return false;
}

void GeometryPicker::pick(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
	osgViewer::View* view = dynamic_cast<osgViewer::View*> (&aa);
	osg::Camera *mainCam = view->getCamera();

	osgUtil::LineSegmentIntersector::Intersections hits;
	if (view->computeIntersections(mainCam, osgUtil::Intersector::WINDOW, ea.getX(), ea.getY(), hits))
	{
		osgUtil::LineSegmentIntersector::Intersection intersection = *hits.begin();

		std::string objectName = "";
		osg::MatrixTransform *objectNode = NULL;

		// auto nearIntersection = *std::prev(hits.end());
		auto nearIntersection = intersection;

		osg::NodePath& nodePath = nearIntersection.nodePath;
		int nNodeSize = static_cast<int> (nodePath.size());
		bool clickedOnSkyBox = false;
		if (nNodeSize > 0)
		{
			bool foundNode = false;
			for (int i = nNodeSize - 1; i >= 0; i--)
			{
				osg::Node *temp = nodePath[i];
				std::string name = temp->getName();
				if (name.substr(0, 9) == "Transform")
				{
					objectName = name.substr(10, name.size() - 1);

					// For unpickable/unmovable objects
					if (objectName.substr(0, 1) == "*") {
						objectName = "";
						continue;
					}

					if (temp->asTransform() == NULL || temp->asTransform()->asMatrixTransform() == NULL)
					{
						OSG_WARN << "GeometryPicker : node " << temp->getName() << " is not a MatrixTransform" << std::endl;
						return;
					}
					objectNode = static_cast<osg::MatrixTransform *>(temp);
					foundNode = true;
					break;
				}
				else if (name == "__LightVisualizerBillBoard")
				{
					// TODO: 
					osg::Drawable *drawable = nearIntersection.drawable;
					drawable->dirtyBound();
				}
				else if (name.substr(0, 6) == "Skybox")
				{
					clickedOnSkyBox = true;
				}
			}
			if (!foundNode && !clickedOnSkyBox)
			{
				OSG_WARN << "GeometryPicker: MatrixTransform node not found" << std::endl;
				return;
			}

			if (clickedOnSkyBox == true)
			{
				GeometryObjectManipulator::detachManipulator();
			}
			else
			{ 
					std::cout << "Picking object : " << objectName << std::endl;
					GeometryObjectManipulator::
						changeCurrentNode(objectNode);
			}
		}
	}

}
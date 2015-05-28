#include "SparkUpdatingHandler.h"
#include <osg/ValueObject>
#include <osgViewer/View>

bool SparkUpdatingHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
	if (!view) return false;

	osg::Camera* camera = view->getCamera();
	if (!camera) return false;

	double time = view->getFrameStamp()->getSimulationTime();
	if (ea.getEventType() == osgGA::GUIEventAdapter::FRAME)
	{
		osg::Vec3d eye, center, up;
		camera->getViewMatrixAsLookAt(eye, center, up);

		for (std::vector<SparkObject>::iterator itr = _sparks.begin(); itr != _sparks.end(); ++itr)
		{
			SparkDrawable* spark = itr->_spark.get();
			if (!spark) continue;

			osg::Transform* trackee = itr->_trackee.get();
			if (trackee)
			{
				itr->_transformMatrix = computeTransformMatrix(spark, trackee);

				osg::Matrix matrix; trackee->computeLocalToWorldMatrix(matrix, NULL);

				osg::Vec3 osgStart;
				const osg::Vec3& osgEnd = matrix.getTrans();
				bool hasValue = trackee->getUserValue<osg::Vec3>("pos", osgStart);
				trackee->setUserValue("pos", osgEnd);

				if (hasValue) {
					SPK::Vector3D start(osgStart.x(), osgStart.y(), osgStart.z());
					SPK::Vector3D end(osgEnd.x(), osgEnd.y(), osgEnd.z());

					if (spark->getNumParticleSystems() > 0) {
						SPK::System *system = spark->getParticleSystem(0);
						SPK::Group *group = system->getGroup(0);
						SPK::Model *model = group->getModel();
						SPK::Emitter *emitter = group->getEmitter(0);
						group->addParticles(start, end, emitter, 0.025f);
						
						float step = ea.getTime();

						model->setParam(SPK::PARAM_RED, 0.6f + 0.4f * sin(step));
						model->setParam(SPK::PARAM_GREEN, 0.6f + 0.4f * sin(step + 3.14159f * 2.0f / 3.0f));
						model->setParam(SPK::PARAM_BLUE, 0.6f + 0.4f * sin(step + 3.14159f * 4.0f / 3.0f));
					}
				}
			}
			spark->update(time, eye);
		}
	}
	return false;
}

int SparkUpdatingHandler::getSparkIndex(SparkDrawable *spark)
{
	for (int i = 0; i < _sparks.size(); i++)
	{
		if (_sparks[i]._spark == spark)
		{
			return i;
		}
	}

	OSG_WARN << "SparkUpdatingHandler:: spark not found " << std::endl;
	return -1;
}

osg::Matrix SparkUpdatingHandler::computeTransformMatrix(SparkDrawable* spark, osg::Transform* trackee)
{
	osg::Node* sparkGeode = (spark->getNumParents() > 0 ? spark->getParent(0) : NULL);
	if (!sparkGeode)
		return osg::Matrix::identity();
	else if (!sparkGeode->getNumParents() || !trackee->getNumParents())
		return osg::Matrix::identity();
	else if (sparkGeode->getParent(0) == trackee->getParent(0))
		return osg::Matrix::identity();

	// Collect the parent paths, ignoring the last one (the spark/trackee itself)
	osg::NodePath sparkPath = sparkGeode->getParentalNodePaths().at(0); 
	sparkPath.pop_back();
	osg::NodePath trackeePath = trackee->getParentalNodePaths().at(0);  
	trackeePath.pop_back();
	return computeLocalToWorld(trackeePath) * computeWorldToLocal(sparkPath);
}

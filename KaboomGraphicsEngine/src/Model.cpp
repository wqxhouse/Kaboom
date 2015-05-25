
#include "Model.h"
#include "Core.h"
#include "GeometryCache.h"
#include <osgDB/ReadFile>

Model::Model() 
{
	transform = new osg::MatrixTransform();
}

Model::Model(int type_id) 
{
	transform = new osg::MatrixTransform();

	GeometryCache* cache = Core::getWorldRef().getGeometryCache();
	loadModel(type_id);

	transform->setMatrix(cache->getMatrixById(101));
	playAnimation();
}

Model::Model(std::string& modelName) 
{
	transform = new osg::MatrixTransform();
	loadModel(modelName);
	playAnimation();
}

bool Model::loadModel(int type_id) 
{
	osg::ref_ptr<osg::Node> tmp = Core::getWorldRef().getGeometryCache()->getNodeById(type_id);
	return loadModelHelper(tmp);
}

bool Model::loadModel(std::string& modelName) 
{
	// Make sure model is loaded
	osg::ref_ptr<osg::Node> tmp = Core::getWorldRef().getGeometryCache()->getNodeByFileName(modelName);
	return loadModelHelper(tmp);
}

bool Model::loadModelHelper(osg::ref_ptr<osg::Node> origNode) 
{
	osg::ref_ptr<osg::Node> model = dynamic_cast<osg::Node*>(origNode->clone(osg::CopyOp::DEEP_COPY_ALL));

	if (!model) {
		printf("<Model::loadAnimations> model is null\n");
		return false;
	}

	// Try getting the animation manager from the update callback of the model root
	animManager = dynamic_cast<osgAnimation::BasicAnimationManager*>
		(model->getUpdateCallback());
	if (!this->animManager) {
		transform->setUpdateCallback(animManager.get());
	}

	// Create a node that holds the model with the animation manager
	transform->addChild(model.get());

	return true;
}

osg::ref_ptr<osg::MatrixTransform> Model::getRootNode() 
{
	return transform;
}

void Model::playAnimation(std::string& animName) {
	if (!this->animManager) {
		printf("<Model::playAnimation> animManager is null\n");
	}

	// Iterate over all animations recorded in the manager
	const osgAnimation::AnimationList& animations =
		animManager->getAnimationList();

	// Find the animation to play
	for (unsigned int i = 0; i<animations.size(); ++i)
	{
		const std::string& name = animations[i]->getName();
		//std::cout << "name:" << name << std::endl;
		if (name == animName) {
			animManager->playAnimation(animations[i].get());
			break;
		}
	}
}

void Model::stopAnimation() 
{
	if (!this->animManager) {
		printf("<Model::stopAnimation> animManager is null\n");
	}
	animManager->stopAll();
	//manager->stopAnimation();
}





#include "Model.h"
#include <osgDB/ReadFile>

Model::Model() {
	transform = new osg::MatrixTransform();
}

Model::Model(std::string& modelName) {
	transform = new osg::MatrixTransform();
	loadModel(modelName);
}

bool Model::loadModel(std::string& modelName) {
	// Make sure model is loaded
	osg::ref_ptr<osg::Node> model = osgDB::readNodeFile(modelName);
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

osg::ref_ptr<osg::MatrixTransform> Model::getRootNode() {
	return transform;
}

//void Model::setPosition(osg::Vec3& v) {
//	transform->setPosition(v);
//}


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

void Model::stopAnimation() {
	if (!this->animManager) {
		printf("<Model::stopAnimation> animManager is null\n");
	}
	animManager->stopAll();
	//manager->stopAnimation();
}




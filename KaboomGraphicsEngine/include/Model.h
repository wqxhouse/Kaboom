
#ifndef MODEL_H
#define MODEL_H

#include <Windows.h>
#include <osg/MatrixTransform>
#include <osgAnimation/BasicAnimationManager>

/*
* Notes:
* - Root node is a MatrixTransform
* - Child node is the loaded model's root
* - AnimationManager is set as updateCallback of root node
* - To play animation, need to pass in the name of the animation
* - stopAnimation() currently stops all animation

Example Usage:

	string str_model, str_anim1;
	ConfigSettings* config = ConfigSettings::config;
	config->getValue(ConfigSettings::str_model, str_model);
	config->getValue(ConfigSettings::str_anim1, str_anim1);

	Model char1 = Model(str_model);
	//char1.loadModel(str_model);

	char1.playAnimation(str_anim1);
	//char1.stopAnimation();

	osg::Matrix transformMat1;
	transformMat1.makeTranslate(-5.0f, 0.0f, 0.0f);
	char1.getRootNode()->setMatrix(transformMat1);

	// Attach to node
	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(char1.getRootNode());


Current Usage:

	std::string str_model, str_anim1;
	ConfigSettings* config = ConfigSettings::config;
	config->getValue(ConfigSettings::str_model, str_model);
	config->getValue(ConfigSettings::str_anim1, str_anim1);

	Model char1 = Model(str_model);
	char1.playAnimation(str_anim1);

	osg::Matrix transformMat1;
	//transformMat1.makeTranslate(-5.0f, 0.0f, 0.0f);
	char1.getRootNode()->setMatrix(transformMat1);

	osg::Group *playerNode = new osg::Group;

	playerNode->addChild(char1.getRootNode());

	player->attachComponent(new SceneNodeComponent(playerNode));

*/

class Model
{
public:
	Model();							// Default: no model
	Model(std::string& modelName);		// For loading a model

	bool loadModel(std::string& modelName);
	osg::ref_ptr<osg::MatrixTransform> getRootNode();
	//void setPosition(osg::Vec3& v);
	void playAnimation(std::string& animName);
	void stopAnimation();

private:
	osg::ref_ptr<osgAnimation::BasicAnimationManager> animManager;
	osg::ref_ptr<osg::MatrixTransform> transform;

};

#endif
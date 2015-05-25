
#ifndef MODEL_H
#define MODEL_H

#include <Windows.h>
#include <osg/MatrixTransform>
#include <osgAnimation/BasicAnimationManager>

class Model
{
public:
	Model();							// Default: no model
	Model(int type_id);					// Loading a model by type id
	Model(std::string& modelName);		// For loading a model

	bool loadModel(int type_id);
	bool loadModel(std::string& modelName);
	osg::ref_ptr<osg::MatrixTransform> getRootNode();
	void playAnimation(std::string& animName = std::string("Default Take"));
	void stopAnimation();

private:
	bool loadModelHelper(osg::ref_ptr<osg::Node> origNode);
	osg::ref_ptr<osgAnimation::BasicAnimationManager> animManager;
	osg::ref_ptr<osg::MatrixTransform> transform;

};

#endif
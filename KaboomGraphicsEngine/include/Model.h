#ifndef MODEL_H
#define MODEL_H

#include <Windows.h>
#include <osg/MatrixTransform>
#include <osgAnimation/BasicAnimationManager>

class Model
{
public:
	Model();							// Default: no model
	Model(int type_id, bool hasAnim,	// Loading a model by type id
		osgAnimation::Animation::PlayMode playMode = osgAnimation::Animation::LOOP);
	Model(std::string& modelName);		// For loading a model

	bool loadModel(int type_id);
	bool loadModel(std::string& modelName);
	osg::ref_ptr<osg::MatrixTransform> getRootNode();

	void addAnimationById(int type_id, osgAnimation::Animation::PlayMode playMode =
		osgAnimation::Animation::LOOP);
	void playAnimation(std::string& animName = std::string("Default Take"));
	void playAnimation(int type_id, std::string& animName = std::string("Default Take"));
	void stopAnimation();

private:
	bool loadModelHelper(osg::ref_ptr<osg::Node> origNode, osg::ref_ptr<osg::Group> newNode,
		osg::ref_ptr<osgAnimation::BasicAnimationManager>& animManager);

	osg::ref_ptr<osgAnimation::BasicAnimationManager> _animManager;
	osg::ref_ptr<osg::MatrixTransform> _root;

	// Used for multiple animations
	int _curr_type_id;
	std::unordered_map<int, osg::ref_ptr<osg::MatrixTransform>> _modelMap;
	std::unordered_map<int, osg::ref_ptr<osgAnimation::BasicAnimationManager>> _animManagerMap;
};

#endif
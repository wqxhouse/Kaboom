#include "ModelCache.h"
#include "Core.h"			// For animation type-id constants
#include <algorithm>		// find()

ModelCache::ModelCache()
{
	// Do nothing
}

ModelCache::~ModelCache() {
	for (int i = 0; i < _available.size(); i++) {
		delete _available[i];
	}

	for (int i = 0; i < _taken.size(); i++) {
		delete _taken[i];
	}

	_available.clear();
	_taken.clear();
}

void ModelCache::addModels(int num)
{
	for (int i = 0; i < num; i++) {
		Model *model;
		model = new Model(IDLE, true, osgAnimation::Animation::PPONG);
		model->addAnimationById(RUNNING);

		_available.push_back(model);
	}
}

Model* ModelCache::getModel() {
	
	if (_available.size() == 0) {
		// return null if no available model
		printf("(ModelCache::getModel) No available models\n");
		return NULL;
	}
	else {
		Model* retModel = _available.back();

		// Transfer model from _available to _taken
		_available.pop_back();
		_taken.push_back(retModel);

		return retModel;
	}
}

void ModelCache::releaseModel(Model* model) {
	int pos = find(_taken.begin(), _taken.end(), model) - _taken.begin();
	// std::vector<Model*>::iterator i = find(_taken.begin(), _taken.end(), model);

	if ((pos < 0) || (pos > _taken.size())) {
		printf("(ModelCache::releaseModel) Model being released does not belong here");
		return;
	}

	// Transfer model from _taken to _available
	_available.push_back(model);
	_taken.erase(_taken.begin() + pos);
}

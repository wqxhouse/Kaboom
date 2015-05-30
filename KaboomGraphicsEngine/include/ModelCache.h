#ifndef MODELCACHE_H
#define MODELCACHE_H

#include <Windows.h>
#include "Model.h"

class ModelCache
{
public:
	ModelCache();
	~ModelCache();

	void addModels(int num);
	Model* getModel();
	void releaseModel(Model* model);

private:
	std::vector<Model*> _available;
	std::vector<Model*> _taken;
};

#endif
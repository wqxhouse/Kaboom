#pragma once

#include <Core.h>
#include <Model.h>
#include <ModelCache.h>

#include <components/Component.h>

class ModelComponent : public Component {
public:
	ModelComponent() {
		model = Core::getModelCache().getModel();
	}

	~ModelComponent() {
		Core::getModelCache().releaseModel(model);
	}

	inline Model* getModel() const {
		return model;
	}

	inline void setModel(Model* model) {
		this->model = model;
	}

private:
	Model* model;
};

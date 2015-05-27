#pragma once

#include <Model.h>

#include <components/Component.h>

class ModelComponent : public Component {
public:
	ModelComponent(Model* model)
		: model(model) {
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

#pragma once

#include <TrailingEffect.h>

#include <components/Component.h>

class TrailingEffectComponent : public Component {
public:
	TrailingEffectComponent(TrailingEffect* effect)
			: effect(effect) {
	}

	inline TrailingEffect* getTrailingEffect() const {
		return effect;
	}

	inline void setTrailingEffect(TrailingEffect* effect) {
		this->effect = effect;
	}

private:
	TrailingEffect* effect;
};

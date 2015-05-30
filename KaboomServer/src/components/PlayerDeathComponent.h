#pragma once

#include <btBulletDynamicsCommon.h>

#include <components/Component.h>

class PlayerDeathComponent : public Component {
public:
	PlayerDeathComponent(bool isReadyToSpawn = false)
		: isReadyToSpawn(isReadyToSpawn){
	}

	inline const bool getIsReadyToSpawn() const {
		return isReadyToSpawn;
	}

	inline void setIsReadyToSpawn(bool isReadyToSpawn) {
		this->isReadyToSpawn = isReadyToSpawn;
	}

private:
	bool isReadyToSpawn;
};

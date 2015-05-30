#pragma once

#include <btBulletDynamicsCommon.h>

#include <components/Component.h>

class PlayerRespawnComponent : public Component {
public:
	PlayerRespawnComponent(bool isReadyToSpawn = false)
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

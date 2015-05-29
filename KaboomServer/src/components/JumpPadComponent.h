#pragma once

#include <btBulletDynamicsCommon.h>

#include <components/Component.h>

class JumpPadComponent : public Component {
public:
	JumpPadComponent(float x, float y, float z, int launchDuration)
					: launchSpeedVec(x, y, z),
					  launchDuration(launchDuration){
    }

	inline const btVector3 getLaunchSpeedVec() const {
		return launchSpeedVec;
	}
	inline const int getLaunchDuration() const {
		return launchDuration;
	}

	inline void setLaunchSpeedVec(float x, float y, float z) {
		launchSpeedVec.setX(x);
		launchSpeedVec.setY(y);
		launchSpeedVec.setZ(z);
	}

	inline void setLaunchDuration(int launchDuration) {
		this->launchDuration = launchDuration;
	}

private:
	btVector3 launchSpeedVec;
	int launchDuration;

};

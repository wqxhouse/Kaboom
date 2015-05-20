#pragma once

#include <btBulletDynamicsCommon.h>

#include <components/Component.h>

class JumpPadComponent : public Component {
public:
	JumpPadComponent(float launchSpeed, float x, float y, float z, float launchDuration)
					: launchSpeed(launchSpeed), 
					  launchDirection(x, y, z),
					  launchDuration(launchDuration){
    }

    inline const float getLaunchSpeed() const {
		return launchSpeed;
    }
	inline const btVector3 getLaunchDirection() const {
		return launchDirection;
	}
	inline const float getLaunchDuration() const{
		return launchDuration;
	}


	inline void setLaunchSpeed(float launchSpeed) {
		this->launchSpeed = launchSpeed;
	}

	inline void setLaunchDirection(float x, float y, float z) {
		launchDirection.setX(x);
		launchDirection.setY(y);
		launchDirection.setZ(z);
	}

	inline void setLaunchDuration(float launchDuration){
		this->launchDuration = launchDuration;
	}

private:

	float launchSpeed;
	btVector3 launchDirection;
	float launchDuration;

};

#pragma once
#include "Component.h"


class HealthComponent : public Component {
public:
	HealthComponent(int,int);
	~HealthComponent();

	int getHealthAmount() const; 
	int getShieldAmount() const;
	void setHealthAmount(int);
	void addToHealthAmount(int);
	void subtractFromHealthAmount(int);

private:
	int healthAmount;
	int healthCap;
	int shieldAmount;
	int shieldCap;
};

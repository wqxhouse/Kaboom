#pragma once
#include <ctime>
#include <core/Component.h>
#define REFUELRATE 25
#define TANKSIZE 100
#define REFUELTIME 1000

class JetpackComponent : public Component {
public:
	JetpackComponent();
	~JetpackComponent();

	bool activateJetpack();
	void refillJetpack();



private:
	int jumpsLeft;
	clock_t beginTime;
};

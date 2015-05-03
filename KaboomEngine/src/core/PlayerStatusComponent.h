#pragma once
#include "Component.h"
#include "../util/Timer.h"


class PlayerStatusComponent : public Component {
public:
	PlayerStatusComponent();
	~PlayerStatusComponent();

	//Below method is only intend to be use on the Server!
	bool checkIsKnockBacked();
	bool checkIsStaggered();

	//Below method is only intend to be use on the Client!
	bool getIsKnockBacked() const;
	bool getIsStaggered() const;

	void setIsKnockBacked(bool);
	void setIsStaggered(bool);

	Timer &getKnockBackTimer();
	Timer &getStaggerTimer();

private:
	bool isKnockBacked;
	bool isStaggered;
	Timer knockBackTimer;
	Timer staggerTimer;

};

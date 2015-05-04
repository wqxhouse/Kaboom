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
	bool checkIsDamaged();

	//Below method is only intend to be use on the Client!
	bool getIsKnockBacked() const;
	bool getIsStaggered() const;
	bool getIsDamaged() const;

	void setIsKnockBacked(bool);
	void setIsStaggered(bool);
	void setIsDamaged(bool);

	Timer &getKnockBackTimer();
	Timer &getStaggerTimer();
	Timer &getDamageTimer();

private:
	bool isKnockBacked;
	bool isStaggered;
	bool isDamaged;

	Timer knockBackTimer;
	Timer staggerTimer;
	Timer damageTimer;

};

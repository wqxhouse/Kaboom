#pragma once
#include "GameGUIListener.h"

class OnClickChampSelectListener :
	public GameGUIListener
{
public:
	OnClickChampSelectListener(Game *game)
		: GameGUIListener(game){}
	~OnClickChampSelectListener();

	virtual void ProcessEvent(Rocket::Core::Event& event);
	void setNumber(unsigned int &a,unsigned number);
	unsigned int* getNumber();


	void setNameElement(Rocket::Core::Element *);

private:
	unsigned int* charId;
	unsigned int num;
	Rocket::Core::Element *nameElement;
};


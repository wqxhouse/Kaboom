#pragma once
#include "GameGUIListener.h"
class OnClickRemoveLetterListener :
	public GameGUIListener
{
public:
	OnClickRemoveLetterListener(Game *game)
		: GameGUIListener(game) {}
	~OnClickRemoveLetterListener();
	void setName(std::string *na);
	std::string* getName();
	void setNameElement(Rocket::Core::Element *);

	virtual void ProcessEvent(Rocket::Core::Event& event);

private:
	std::string* name;
	Rocket::Core::Element *nameElement;
};


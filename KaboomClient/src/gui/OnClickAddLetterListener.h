#pragma once
#include "GameGUIListener.h"

class OnClickAddLetterListener :
	public GameGUIListener
{
public:
	OnClickAddLetterListener(Game *game)
		: GameGUIListener(game){}
	~OnClickAddLetterListener();

	virtual void ProcessEvent(Rocket::Core::Event& event);
	void setLetter(char let);
	char getLetter();
	void setName(std::string *na);
	std::string* getName();
	void setNameElement(Rocket::Core::Element *);

private:
	char letter;
	std::string *name;
	Rocket::Core::Element *nameElement;
};


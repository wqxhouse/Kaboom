#include "stdafx.h"
#include "OnClickAddLetterListener.h"

OnClickAddLetterListener::~OnClickAddLetterListener()
{
}

void OnClickAddLetterListener::ProcessEvent(Rocket::Core::Event& event)
{	
	if (name->size() < 24){
		name->push_back(letter);
		nameElement->SetInnerRML(name->c_str());
	}
	std::cout << "test" << std::endl;
}
void OnClickAddLetterListener::setLetter(char let){
	letter = let;
}
char OnClickAddLetterListener::getLetter(){
	return letter;
}
void OnClickAddLetterListener::setName(std::string *na){
	name = na;
}
std::string* OnClickAddLetterListener::getName(){
	return name;
}
void OnClickAddLetterListener::setNameElement(Rocket::Core::Element *doug){
	nameElement = doug;
}
#include "stdafx.h"
#include "OnClickRemoveLetterListener.h"


OnClickRemoveLetterListener::~OnClickRemoveLetterListener()
{
}

void OnClickRemoveLetterListener::ProcessEvent(Rocket::Core::Event& event)
{
	
	if (name->size() > 0){
		name->pop_back();
		nameElement->SetInnerRML(name->c_str());
	}
	std::cout << "test" << std::endl;
}
void OnClickRemoveLetterListener::setName(std::string *na){
	name = na;
}
std::string* OnClickRemoveLetterListener::getName(){
	return name;
}
void OnClickRemoveLetterListener::setNameElement(Rocket::Core::Element *doug){
	nameElement = doug;
}
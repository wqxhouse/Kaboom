#include "OnClickListener.h"

void OnClickListener::ProcessEvent(Rocket::Core::Event& event) 
{
	setGameState(CONNECT_TO_SERVER);
	std::cout << "marty was clicked" << std::endl;
}
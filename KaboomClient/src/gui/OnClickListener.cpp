#include "OnClickListener.h"

void OnClickListener::ProcessEvent(Rocket::Core::Event& event) 
{
	setGameState(mode);
	std::cout << "marty was clicked" << std::endl;
}
void OnClickListener::setMode(GameStateMachine g){
	mode = g;
}
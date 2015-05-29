#pragma once
#include <Rocket/Core/EventListener.h>
#include <Rocket/Core/Element.h>
#include <network/Event.h>
#include "../core/GameStateMachine.h"

class Game;
class GameGUIListener : public Rocket::Core::EventListener
{
public:
	GameGUIListener(Game *game);

	/// Process the incoming Event
	virtual void ProcessEvent(Rocket::Core::Event& event) = 0;

	/// Called when the listener has been attached to a new Element
	//virtual void OnAttach(Rocket::Core::Element* ROCKET_UNUSED_PARAMETER(element));
	/// Called when the listener has been detached from a Element
	//virtual void OnDetach(Rocket::Core::Element* ROCKET_UNUSED_PARAMETER(element));

protected:
	friend class Game;
	void setGameState(GameStateMachine state);
	Game *_game;
};
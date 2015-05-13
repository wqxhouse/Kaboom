#include "HealthEventHandler.h"

#include <osg/MatrixTransform>

#include <Core.h>
#include <GeometryObject.h>
#include <components/HealthComponent.h>
#include <core/Entity.h>
#include <network/HealthEvent.h>

#include "../components/SceneNodeComponent.h"
#include "../core/Game.h"

HealthEventHandler::HealthEventHandler(Game *game)
        : game(game) {
}

void HealthEventHandler::handle(const Event &e) const {
	const HealthEvent &evt = static_cast<const HealthEvent &>(e);

	Entity *player = game->getEntityManager().getEntity((game->getGameClient().getCurrentPlayerEntityId()));
	HealthComponent *health = player->getComponent<HealthComponent>();
	health->setHealthAmount(evt.getHealthAmount());

	game->getGameGUIEventHandler().handle(evt, health);

	/*Rocket::Core::ElementDocument *window1 = game->getLibRocket()->getWindow(0);
	Rocket::Core::Element * table = window1->GetFirstChild();
	Rocket::Core::Element *tr = table->GetFirstChild();
	for (int i = 0; i < tr->GetNumChildren(); i++){
	printf("\nthe health is at %d\n", health->getHealthAmount());
	if (health->getHealthAmount() / 20 <= i){
	tr->GetChild(i)->SetProperty("background-color", "blue");
	continue;
	}
	tr->GetChild(i)->SetProperty("background-color","red");
	}*/
	//win->GetChild(1);
}

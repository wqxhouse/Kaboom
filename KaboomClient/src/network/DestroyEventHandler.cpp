#include "DestroyEventHandler.h"

#include <network/DestroyEvent.h>

#include "../components/TrailingEffectComponent.h"
#include "../core/Game.h"

DestroyEventHandler::DestroyEventHandler(Game *game)
        : game(game) {
}

void DestroyEventHandler::handle(const Event &e) const {
    const DestroyEvent &evt = static_cast<const DestroyEvent &>(e);

    Entity *entity = game->getEntityManager().getEntity(evt.getEntityId());

    if (entity != nullptr) {
		auto trailingEffectComp = entity->getComponent<TrailingEffectComponent>();

		if (trailingEffectComp != nullptr) {
			delete trailingEffectComp->getTrailingEffect();
		}

        game->removeEntity(entity);
    }
}

#include "DestroyEventHandler.h"

#include <Core.h>
#include <ObjectGlowManager.h>
#include <components/WeaponPickupComponent.h>
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

        if (entity->hasComponent<WeaponPickupComponent>()) {
            auto obj = game->getGeometryManager()->getGeometryObject(std::to_string(entity->getId()));
            Core::getWorldRef().getObjectGlowManager()->removeGlowGeometryObject(obj);
        }

        game->removeEntity(entity);
    }
}

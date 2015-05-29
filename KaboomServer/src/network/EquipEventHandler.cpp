#include "EquipEventHandler.h"

#include <components/EquipmentComponent.h>
#include <core/EntityManager.h>
#include <network/EquipEvent.h>

#include "../core/Game.h"

EquipEventHandler::EquipEventHandler(Game *game)
        : game(game) {
}

void EquipEventHandler::handle(const Event &e) const {
    const EquipEvent &evt = static_cast<const EquipEvent &>(e);

    EntityManager &entityManager = game->getEntityManager();
    Entity *entity = entityManager.getEntity(evt.getEntityId());

    auto equipComp = entity->getComponent<EquipmentComponent>();

    if (equipComp != nullptr) {
        equipComp->setType(evt.getType());
    }
}

#include "DisconnectEventHandler.h"

#include <core/EntityManager.h>
#include <network/DisconnectEvent.h>

#include "../core/Game.h"

DisconnectEventHandler::DisconnectEventHandler(Game *game)
        : game(game) {
}

DisconnectEventHandler::~DisconnectEventHandler() {
}

void DisconnectEventHandler::handle(const Event &e) const {
    const DisconnectEvent &evt = static_cast<const DisconnectEvent &>(e);

    EntityManager &entityManager = game->getEntityManager();
    Entity *player = entityManager.getEntity(evt.getPlayerId());

    game->getGameServer().sendDisconnectEvent(player);

    PhysicsComponent *physComp = player->getComponent<PhysicsComponent>();
    game->getWorld().removeRigidBody(physComp->getRigidBody());

    game->getEntityManager().destroyEntity(player->getId());
}

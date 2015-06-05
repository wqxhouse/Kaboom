#include "PlayerDeathEventHandler.h"

#include <Core.h>
#include <GeometryCache.h>
#include <network/PlayerDeathEvent.h>
#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <GeometryObject.h>

#include "../components/ModelComponent.h"
#include "../core/Game.h"

PlayerDeathEventHandler::PlayerDeathEventHandler(Game *game)
        : game(game) {
}

void PlayerDeathEventHandler::handle(const Event &e) const {
    const PlayerDeathEvent &evt = static_cast<const PlayerDeathEvent &>(e);

    if (game->getCurrentPlayer()->getId() == evt.getPlayerId()) {
        game->playerAlive = false;
        game->voiceSource->setSound(game->voiceMap->at(Game::VoiceActing::DEATH_1));
        game->deathTime = std::chrono::high_resolution_clock::now();
        game->voiceSource->setGain(1);
        game->voiceSource->play();
        game->getGameGUIEventHandler()->handle(evt);
    }

    const auto player = game->getPlayers().at(evt.getPlayerId());

    if (player->getEntity() != nullptr) {
        const std::string name = std::to_string(player->getEntity()->getId());
        game->getGeometryManager()->getGeometryObject(name)->setTranslate(osg::Vec3(0, 0, -10000));
    }
}

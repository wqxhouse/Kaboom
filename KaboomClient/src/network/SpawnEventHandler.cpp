#include "SpawnEventHandler.h"

#include <core/EntityType.h>
#include <network/SpawnEvent.h>
#include <components/PositionComponent.h>
#include <ParticleEffectManager.h>
#include <TrailingEffect.h>

#include "../core/Game.h"
#include "../components/SceneNodeComponent.h"
#include <GeometryObject.h>

SpawnEventHandler::SpawnEventHandler(Game *game)
        : game(game) {
}

void SpawnEventHandler::handle(const Event &e) const {
    const SpawnEvent &evt = static_cast<const SpawnEvent &>(e);
	EntityManager &entityManager = game->getEntityManager();
    EntityType type = evt.getType();
    Entity *entity = nullptr;
	
	
    if ((type & CAT_MASK) == CAT_CHARACTER) {
        entity = game->getCharacterFactory().createCharacter(
                evt.getEntityId(),
                evt.getType(),
                evt.getPosition(),
                evt.getRotation());
    } else if ((type & CAT_MASK) == CAT_BOMB) {
        entity = game->getBombFactory().createBomb(
                evt.getEntityId(),
                evt.getType(),
                evt.getPosition(),
                evt.getRotation());

		game->source = new Source;
        Entity *player = game->getCurrentPlayer()->getEntity();
		if (player != nullptr){


			PositionComponent *playerPosComp = player->getComponent<PositionComponent>();
			PositionComponent *entityPosComp = entity->getComponent<PositionComponent>();
            const Vec3 &playerPos = playerPosComp->getPosition();
            const Vec3 &entityPos = entityPosComp->getPosition();

            double x = (double)(-playerPos.x + entityPos.x);
            double y = (double)(-playerPos.y + entityPos.y);
            double z = (double)(-playerPos.z + entityPos.z);

			switch (type&(~CAT_BOMB)){
			case KABOOM_V2:
				game->source->setSound(game->sounds->at(KABOOM_FIRE).get());
				break;
			default:
				game->source->setSound(game->sounds->at(BASIC).get());
				printf("unknown bomb type gets no sound\n");
				break;
			}
			game->source->setRolloffFactor(sqrt(x*x + y*y + z*z));
			game->source->setGain(1);
			game->source->setLooping(false);
			osgAudio::AudioEnvironment::instance()->update();
			game->source->play();
		}
    }

    if (entity != nullptr) {
        game->addEntity(entity);

		// TODO: refactor addEntity. or the following code must be called after addEntity
		// handle particle effect of bomb spawning. 
		//if ((type & CAT_MASK) == CAT_BOMB)
		//{
		//	TrailingEffect *trailingParticle = static_cast<TrailingEffect *>(
		//		game->getParticleEffectManager()->getParticleEffect(ParticleEffectManager::TRAILING));

		//	GeometryObject *geomObj = game->getGeometryManager()->getGeometryObject(std::to_string(entity->getId()));
		//	trailingParticle->setTrailedObject(geomObj);
		//	trailingParticle->run();
		//}
    }
}

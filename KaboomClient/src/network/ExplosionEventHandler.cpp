#include "ExplosionEventHandler.h"

#include <network/ExplosionEvent.h>
#include <core/PositionComponent.h>
#include <core/RotationComponent.h>
#include "../core/Game.h"

ExplosionEventHandler::ExplosionEventHandler(Game *game)
        : game(game) {
}

void ExplosionEventHandler::handle(const Event &e) const {
    const ExplosionEvent &evt = static_cast<const ExplosionEvent &>(e);

    EntityManager &entityManager = game->getEntityManager();
    Entity *bomb = entityManager.getEntity(evt.getBombId());
	Entity *player = entityManager.getEntity(game->getGameClient().getCurrentPlayerEntityId());

    // TODO: Handle explosion effect.
	try{
		PositionComponent *pos = bomb->getComponent<PositionComponent>();
		RotationComponent *ro = player->getComponent<RotationComponent>();
		float yaw=ro->getYaw();
		float pitch=ro->getPitch();
		PositionComponent *playerPos = player->getComponent<PositionComponent>();
		double x =(double) (playerPos->getX() - pos->getX());
		double y = (double)(playerPos->getY() - pos->getY());
		double z =(double) (playerPos->getZ() - pos->getZ());
		std::cout << " x " << x << " y " << y << " z " << z << std::endl;
		//game->source->rewind();
		game->source=new Source;
		game->source->setSound(game->sample.get());
		//game->source->setReferenceDistance(sqrt(x*x + y*y + z*z));
		game->source->setRolloffFactor(sqrt(x*x + y*y + z*z));
		game->source->setPosition(x,y,z);		
		osgAudio::AudioEnvironment::instance()->update();
		game->source->play();
	}
	catch (Error e) {
		std::cerr << e << "\n";
	}

    game->removeEntity(bomb);
}

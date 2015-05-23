#include "ExplosionEventHandler.h"

#include <cmath>

#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <network/ExplosionEvent.h>
#include <ParticleEffectManager.h>
#include <ExplosionEffect.h>

#include "../core/Game.h"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795029
#endif

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
		PositionComponent *bombPosComp = bomb->getComponent<PositionComponent>();
		RotationComponent *ro = player->getComponent<RotationComponent>();
//		float yaw=ro->getYaw();
//		float pitch=ro->getPitch();
        PositionComponent *playerPosComp = player->getComponent<PositionComponent>();
        const Vec3 &playerPos = playerPosComp->getPosition();
        const Vec3 &bombPos = bombPosComp->getPosition();
        double x = (double)(-playerPos.x + bombPos.x);
        double y = (double)(-playerPos.y + bombPos.y);
        double z = (double)(-playerPos.z + bombPos.z);
		std::cout << " x " << x << " y " << y << " z " << z << std::endl;
		//game->source->rewind();
		game->source=new Source;
		game->source->setSound(game->sounds->at(KABOOM_EXPLODE));
		game->source->setGain(1);
		game->source->setLooping(false);
		//game->source->setReferenceDistance(sqrt(x*x + y*y + z*z));
		game->source->setRolloffFactor(sqrt(x*x + y*y + z*z));
		float mag = sqrt(x*x + y*y + z*z);
		x = x / mag;
		y = y / mag;
		z = z / mag;
		float p = 1;
//		float phi=pitch;
//		float theta=yaw;
//		float x2 = p*sin(phi)*cos(theta);
//		float y2 = p*sin(phi)*sin(theta);
//		float z2 = p*cos(phi);
//		float mag2 = sqrt(x2*x2+y2*y2+z2*z2);
		mag = sqrt(x*x + y*y + z*z);
//		float radian = (x*x2 + y*y2 + z*z2) / (mag*mag2);
//		printf("before acos %f \n", radian);
//		radian = acos(radian);
		//game->source->setPosition(cos(radian), 0, sin(radian));
//		std::cout << " x2 " << x2 << " y2 " << y2 << " z2 " << z2 << std::endl;
//		printf("radian is %f \n",radian);
		osgAudio::AudioEnvironment::instance()->update();
		game->source->play();

		// TODO: need to refactor the code above... probably abstract out a sound manager
		// trigger explosion particle effect
		ExplosionEffect *explosionEffect = 
			static_cast<ExplosionEffect *>(game->getParticleEffectManager()->getParticleEffect(ParticleEffectManager::EXPLOSION));
		// TODO: can add some randomness by applying a random quat
		// explosionEffect->setExplosionRadius(2);
		osg::Vec3 bombPosVec = bombPos.getOsgVec3();
		osg::Matrix mm;
		mm.makeRotate(1.57, osg::Vec3(0, 1, 0));
		osg::Quat qq = mm.getRotate();
		explosionEffect->run(bombPos.getOsgVec3(), qq);
	}
	catch (Error e) {
		std::cerr << e << "\n";
	}

    //game->removeEntity(bomb); TODO we dont remove the bomb here, destory event will be in another event
}

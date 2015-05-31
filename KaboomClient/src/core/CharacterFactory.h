#pragma once

#include <math/Quat.h>
#include <osgAudio/Sample.h>
using namespace osgAudio;
class Entity;
enum EntityType;
class EntityManager;
class Vec3;

class CharacterFactory {
public:
    CharacterFactory(EntityManager &entityManager);
	void setWalkingSample(osg::ref_ptr<Sample> walking);

    Entity *createCharacter(
            unsigned int id,
            EntityType characterType,
            const Vec3 &position,
            Quat rotation) const;

private:
    EntityManager &entityManager;
	osg::ref_ptr<Sample> walk;
};

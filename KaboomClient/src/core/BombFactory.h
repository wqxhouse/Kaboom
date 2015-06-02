#pragma once

#include <math/Quat.h>

class Entity;
enum EntityType;
class EntityManager;
class Vec3;

class BombFactory {
public:
    BombFactory(EntityManager &entityManager);

    Entity *createBomb(
            unsigned int id,
            EntityType type,
            const Vec3 &position,
            Quat rotation) const;

private:
    EntityManager &entityManager;
	osg::ref_ptr<osg::Node> _kaboom2_0;
	osg::ref_ptr<osg::Node> _timer;
	osg::ref_ptr<osg::Node> _remote;
};

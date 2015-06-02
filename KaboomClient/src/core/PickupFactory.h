#pragma once

#include <math/Quat.h>

class Entity;
enum EntityType;
class EntityManager;
class Vec3;

class PickupFactory {
public:
	PickupFactory(EntityManager &entityManager);

    Entity *createPickup(
            unsigned int id,
            EntityType type,
            const Vec3 &position,
            Quat rotation) const;

private:
    EntityManager &entityManager;
	osg::ref_ptr<osg::Node> _kaboom2_0_ammo;
	osg::ref_ptr<osg::Node> _timer_ammo;
	osg::ref_ptr<osg::Node> _remote_ammo;
	osg::ref_ptr<osg::Node> _health_pack;
};

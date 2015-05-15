#pragma once

#include <systems/EntitySystem.h>
#include "../core/Game.h"

// Marks all player entity as dead, but not delete the entity
class DeathSystem : public EntitySystem {
public:
	DeathSystem(Game* game);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);

private:
	Game *game;
};

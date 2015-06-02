#pragma once

#include "EntityProcessingSystem.h"

class Game;

class JumpPadSystem : public EntityProcessingSystem {
public:
	JumpPadSystem(Game *game);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);

};

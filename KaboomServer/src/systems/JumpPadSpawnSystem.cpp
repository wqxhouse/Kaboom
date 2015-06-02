#include "JumpPadSpawnSystem.h"

#include "../core/Game.h"

JumpPadSpawnSystem::JumpPadSpawnSystem(Game *game)
	: game(game) {
}

void JumpPadSpawnSystem::process() {
	auto &configs = game->getSpawnPointConfigs();
    auto &jumpPadSpawnNames = game->getJumpPadSpawnPointList();

	for (auto jumpPadSpawnName : jumpPadSpawnNames) {
		auto config = configs[jumpPadSpawnName];

		EntityType entityType = static_cast<EntityType>(config.getUint("id"));

		Vec3 boxSize =		   Vec3(config.getVec3("box-size").x(), config.getVec3("box-size").y(), config.getVec3("box-size").z());
		Vec3 positionVec =	   Vec3(config.getVec3("position").x(), config.getVec3("position").y(), config.getVec3("position").z());
		Vec3 launchSpeedVec =  Vec3(config.getVec3("launch-speed").x(), config.getVec3("launch-speed").y(), config.getVec3("launch-speed").z());
		Entity* jumpPadEntity = game->getJumpPadFactory().createJumpPad(entityType,
															 boxSize,
															 positionVec,
															 launchSpeedVec,
															 config.getInt("launch-duration"));
		game->addEntity(jumpPadEntity);


	}
	jumpPadSpawnNames.clear();//once we have created it, we never need to create jumpPad again
}

#include "DefaultCharacterMessageHandler.h"

#include <btBulletDynamicsCommon.h>

#include <components/EquipmentComponent.h>
#include <components/InventoryComponent.h>
#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <core/Entity.h>
#include <math/Vec3.h>
#include <util/Configuration.h>

#include "Attack1Message.h"
#include "Attack2Message.h"
#include "Message.h"
#include "MessageType.h"
#include "NoAttackMessage.h"
#include "../components/CharacterRotationComponent.h"
#include "../components/DetonatorComponent.h"
#include "../components/ExplosionComponent.h"
#include "../components/OwnerComponent.h"
#include "../core/EntityConfigLookup.h"
#include "../core/Game.h"
#include "../math/util.h"

bool DefaultCharacterMessageHandler::handle(const Message &message) const {
    switch (message.getType()) {
        case MessageType::ATTACK1: {
            return handle(static_cast<const Attack1Message &>(message));
        }
        case MessageType::ATTACK2: {
            return handle(static_cast<const Attack2Message &>(message));
        }
        case MessageType::NO_ATTACK: {
            return handle(static_cast<const NoAttackMessage &>(message));
        }
    }

    return false;
}

bool DefaultCharacterMessageHandler::handle(const Attack1Message &message) const {
    Entity *entity = message.getEntity();
    Game *game = message.getGame();

    auto equipComp = entity->getComponent<EquipmentComponent>();
    auto invComp = entity->getComponent<InventoryComponent>();
    auto posComp = entity->getComponent<PositionComponent>();
    auto charRotComp = entity->getComponent<CharacterRotationComponent>();

    // Skip if no such bomb in inventory
    EntityType bombType = equipComp->getType();
    if (!invComp->hasBomb(bombType)) {
        return true;
    }

    // Skip if cooldown is not ready
    Timer &timer = invComp->getTimer(bombType);
    if (invComp->getAmount(bombType) == 0 || !timer.isExpired()) {
        return true;
    }

    // Reset cooldown
    timer.start();

    auto &bombConfig = EntityConfigLookup::get(bombType);
    float launchSpeed = bombConfig.getFloat("launch-speed");

    auto &factory = game->getBombFactory();

    const Vec3 &charPos = posComp->getPosition();
    const Vec3 viewDir = getViewDirection(charRotComp->getRotation());

    // Calculate bomb position and velocity
    Vec3 pos, vel;
    pos = calculateBombSpawnLocation(charPos, viewDir);
    vel.setOsgVec3(viewDir.getOsgVec3() * launchSpeed);

    switch (bombType) {
        case KABOOM_V2: {
            Entity *bomb = factory.createBomb(bombType, pos, vel);
            bomb->attachComponent(new OwnerComponent(entity));
            game->addEntity(bomb);

            invComp->remove(bombType);
            break;
        }
        case TIME_BOMB: {
            const int numBombs = bombConfig.getInt("amount");
            const float deltaAngle = bombConfig.getFloat("delta-angle");

            float currAngle = -deltaAngle * (numBombs / 2);

            if (numBombs % 2 == 0) {
                currAngle += deltaAngle / 2.0f;
            }

            for (int i = 0; i < numBombs; ++i) {
                Vec3 currDir = rotateVector(viewDir, Vec3(0, 0, 1), currAngle);
                Vec3 currPos = calculateBombSpawnLocation(charPos, currDir);
                Vec3 currVel;
                currVel.setOsgVec3(currDir.getOsgVec3() * launchSpeed);

                Entity *bomb = factory.createBomb(bombType, currPos, currVel);
                bomb->attachComponent(new OwnerComponent(entity));
                game->addEntity(bomb);

                currAngle += deltaAngle;
            }

            invComp->remove(bombType);
            break;
        }
        case REMOTE_DETONATOR: {
            auto detonatorComp = entity->getComponent<DetonatorComponent>();

            if (detonatorComp == nullptr) {
                detonatorComp = new DetonatorComponent();
                entity->attachComponent(detonatorComp);
            }

            if (!detonatorComp->isDetonated()) {
                auto &bombs = detonatorComp->getBombs();

                if (bombs.size() < EntityConfigLookup::get(bombType).getInt("max-active")) {
                    Entity *bomb = factory.createBomb(bombType, pos, vel);
                    bomb->attachComponent(new OwnerComponent(entity));

                    bombs.push_back(bomb);
                    game->addEntity(bomb);

                    invComp->remove(bombType);
                }
            }

            break;
        }
    }

    return true;
}

bool DefaultCharacterMessageHandler::handle(const Attack2Message &message) const {
    Entity *entity = message.getEntity();

    auto equipComp = entity->getComponent<EquipmentComponent>();
    auto detonatorComp = entity->getComponent<DetonatorComponent>();
    auto invComp = entity->getComponent<InventoryComponent>();

    EntityType bombType = equipComp->getType();

    if (!invComp->hasBomb(bombType)) {
        return true;
    }

    auto &bombConfig = EntityConfigLookup::get(bombType);

    if (bombType == REMOTE_DETONATOR && detonatorComp != nullptr) {
        if (detonatorComp->isReady() && !detonatorComp->isDetonated()) {
            auto &bombs = detonatorComp->getBombs();
            
            for (auto bomb : bombs) {
                bomb->attachComponent(new ExplosionComponent());
            }

            detonatorComp->setDetonated(true);
        }
    }

    return true;
}

bool DefaultCharacterMessageHandler::handle(const NoAttackMessage &message) const {
    Entity *entity = message.getEntity();

    auto detonatorComp = entity->getComponent<DetonatorComponent>();

    if (detonatorComp != nullptr) {
        if (detonatorComp->isDetonated()) {
            entity->detachComponent<DetonatorComponent>();
            delete detonatorComp;
        } else {
            detonatorComp->setReady(true);
        }
    }

    return true;
}

Vec3 DefaultCharacterMessageHandler::calculateBombSpawnLocation(const Vec3 &pos, const Vec3 &dir) const {
    osg::Vec3 cameraPos = osg::Vec3(dir.x, dir.y, 0.0f);
    cameraPos.normalize();
    cameraPos *= 0.5f;
    cameraPos += pos.getOsgVec3() + osg::Vec3(0.0f, 0.0f, 1.0f);

    Vec3 result;
    result.setOsgVec3(cameraPos);

    return result;
}

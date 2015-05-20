#include "DefaultCharacterMessageHandler.h"

#include <btBulletDynamicsCommon.h>

#include <components/BombContainerComponent.h>
#include <components/EquipmentComponent.h>
#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <core/Entity.h>
#include <math/Vec3.h>
#include <util/Configuration.h>

#include "Attack1Message.h"
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
    auto detonatorComp = entity->getComponent<DetonatorComponent>();
    auto invComp = entity->getComponent<BombContainerComponent>();
    auto posComp = entity->getComponent<PositionComponent>();
    auto charRotComp = entity->getComponent<CharacterRotationComponent>();

    EntityType bombType = equipComp->getType();

    if (!invComp->hasBomb(bombType)) {
        return true;
    }

    auto &bombConfig = EntityConfigLookup::get(bombType);

    if (bombType == REMOTE_DETONATOR && detonatorComp != nullptr) {
        if (detonatorComp->isReady() && !detonatorComp->isDetonated()) {
            detonatorComp->getBomb()->attachComponent(new ExplosionComponent());
            detonatorComp->setDetonated(true);
        }
    } else {
        Timer &timer = invComp->getTimer(bombType);

        if (invComp->getAmount(bombType) > 0 && timer.isExpired()) {
            invComp->removeFromInventory(bombType);
            timer.start();

            btVector3 btViewDir = getViewDirection(charRotComp->getRotation());
            Vec3 viewDir = Vec3(btViewDir.getX(), btViewDir.getY(), btViewDir.getZ());

            float launchSpeed = bombConfig.getFloat("launch-speed");
            const Vec3 &charPos = posComp->getPosition();
            
            auto &factory = game->getBombFactory();

            Vec3 pos;
            Vec3 vel;

            pos.setOsgVec3(charPos.getOsgVec3() + osg::Vec3(0.0f, 0.0f, 1.0f) + viewDir.getOsgVec3());
            vel.setOsgVec3(viewDir.getOsgVec3() * launchSpeed);

            switch (bombType) {
                case KABOOM_V2: {
                    Entity *bombEntity = factory.createBomb(bombType, pos, vel);
                    bombEntity->attachComponent(new OwnerComponent(entity));
                    game->addEntity(bombEntity);
                    break;
                }
                case TIME_BOMB: {
                    const float angle = EntityConfigLookup::get(TIME_BOMB).getFloat("angle");

                    Vec3 pos1, vel1;
                    Vec3 dir1 = rotateVector(viewDir, Vec3(0, 0, 1), -angle);
                    pos1.setOsgVec3(charPos.getOsgVec3() + osg::Vec3(0.0f, 0.0f, 1.0f) + dir1.getOsgVec3());
                    vel1.setOsgVec3(dir1.getOsgVec3() * launchSpeed);

                    Vec3 pos2, vel2;
                    Vec3 dir2 = rotateVector(viewDir, Vec3(0, 0, 1), angle);
                    pos2.setOsgVec3(charPos.getOsgVec3() + osg::Vec3(0.0f, 0.0f, 1.0f) + dir2.getOsgVec3());
                    vel2.setOsgVec3(dir2.getOsgVec3() * launchSpeed);

                    Entity *bombEntity;

                    bombEntity = factory.createBomb(bombType, pos, vel);
                    bombEntity->attachComponent(new OwnerComponent(entity));
                    game->addEntity(bombEntity);

                    bombEntity = factory.createBomb(bombType, pos1, vel1);
                    bombEntity->attachComponent(new OwnerComponent(entity));
                    game->addEntity(bombEntity);

                    bombEntity = factory.createBomb(bombType, pos2, vel2);
                    bombEntity->attachComponent(new OwnerComponent(entity));
                    game->addEntity(bombEntity);
                    break;
                }
                case REMOTE_DETONATOR: {
                    Entity *bombEntity = factory.createBomb(bombType, pos, vel);
                    bombEntity->attachComponent(new OwnerComponent(entity));
                    entity->attachComponent(new DetonatorComponent(bombEntity));
                    game->addEntity(bombEntity);
                    break;
                }
            }
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

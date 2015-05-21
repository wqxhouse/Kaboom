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
    auto invComp = entity->getComponent<BombContainerComponent>();
    auto posComp = entity->getComponent<PositionComponent>();
    auto charRotComp = entity->getComponent<CharacterRotationComponent>();

    EntityType bombType = equipComp->getType();

    if (!invComp->hasBomb(bombType)) {
        return true;
    }

    auto &bombConfig = EntityConfigLookup::get(bombType);

    Timer &timer = invComp->getTimer(bombType);

    if (invComp->getAmount(bombType) > 0 && timer.isExpired()) {
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

        auto bombConfig = EntityConfigLookup::get(bombType);

        switch (bombType) {
            case KABOOM_V2: {
                Entity *bombEntity = factory.createBomb(bombType, pos, vel);
                bombEntity->attachComponent(new OwnerComponent(entity));
                game->addEntity(bombEntity);

                invComp->removeFromInventory(bombType);
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
                    Vec3 currPos, currVel;

                    Vec3 currDir = rotateVector(viewDir, Vec3(0, 0, 1), currAngle);
                    currPos.setOsgVec3(charPos.getOsgVec3() + osg::Vec3(0.0f, 0.0f, 1.0f) + currDir.getOsgVec3());
                    currVel.setOsgVec3(currDir.getOsgVec3() * launchSpeed);

                    Entity *bomb = factory.createBomb(bombType, currPos, currVel);
                    bomb->attachComponent(new OwnerComponent(entity));
                    game->addEntity(bomb);

                    currAngle += deltaAngle;
                }

                invComp->removeFromInventory(bombType);
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

                    if (bombs.size() < EntityConfigLookup::get(REMOTE_DETONATOR).getInt("max-active")) {
                        Entity *bomb = factory.createBomb(bombType, pos, vel);
                        bomb->attachComponent(new OwnerComponent(entity));

                        bombs.push_back(bomb);
                        game->addEntity(bomb);

                        invComp->removeFromInventory(bombType);
                    }
                }

                break;
            }
        }
    }

    return true;
}

bool DefaultCharacterMessageHandler::handle(const Attack2Message &message) const {
    Entity *entity = message.getEntity();

    auto equipComp = entity->getComponent<EquipmentComponent>();
    auto detonatorComp = entity->getComponent<DetonatorComponent>();
    auto invComp = entity->getComponent<BombContainerComponent>();

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

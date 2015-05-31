#include "FakeBombMessageHandler.h"

#include <btBulletDynamicsCommon.h>

#include <components/RotationComponent.h>
#include <core/Entity.h>

#include "Message.h"
#include "MessageType.h"
#include "../components/DetonatorComponent.h"
#include "../components/ExplosionComponent.h"
#include "../components/OwnerComponent.h"
#include "../components/PhysicsComponent.h"
#include "../math/util.h"
#include "../messaging/CollisionMessage.h"
#include "../messaging/TickMessage.h"

bool FakeBombMessageHandler::handle(const Message &message) const {
    switch (message.getType()) {
        case MessageType::COLLISION: {
            return handle(static_cast<const CollisionMessage &>(message));
        }
        case MessageType::TICK: {
            return handle(static_cast<const TickMessage &>(message));
        }
    }

    return false;
}

bool FakeBombMessageHandler::handle(const CollisionMessage &message) const {
    Entity *entity = message.getEntity();
    auto &contactEntities = message.getContactEntities();
    auto physComp = entity->getComponent<PhysicsComponent>();
    auto ownerComp = entity->getComponent<OwnerComponent>();
    Entity *owner = ownerComp->getEntity();

    if (contactEntities.empty()) {
        if (!physComp->getRigidBody()->isStaticObject()) {
            auto rotComp = entity->getComponent<RotationComponent>();

            rotComp->setRotation(Quat());
            physComp->getRigidBody()->setMassProps(0.0f, btVector3(0.0f, 0.0f, 0.0f));
            physComp->getRigidBody()->getWorldTransform().setRotation(btQuaternion::getIdentity());
        }
    } else {
        entity->attachComponent(new ExplosionComponent());
    }

    return true;
}

bool FakeBombMessageHandler::handle(const TickMessage &message) const {
    Entity *entity = message.getEntity();
    auto rotComp = entity->getComponent<RotationComponent>();
    auto physComp = entity->getComponent<PhysicsComponent>();

    btQuaternion btQuat = physComp->getRigidBody()->getWorldTransform().getRotation();
    Quat delta = euler2Quat(2.0f, 0.0f, 0.0f);
    btQuaternion btDelta(delta.x, delta.y, delta.z, delta.w);

    btQuat *= btDelta;

    Quat quat(btQuat.getX(), btQuat.getY(), btQuat.getZ(), btQuat.getW());

    rotComp->setRotation(quat);
    physComp->getRigidBody()->getWorldTransform().setRotation(btQuat);

    return true;
}

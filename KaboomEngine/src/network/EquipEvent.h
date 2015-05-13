#pragma once

#include <ostream>

#include "Event.h"
#include "../core/EntityType.h"

class EquipEvent : public Event {
public:
    EquipEvent(unsigned int entityId = 0, EntityType type = NONE)
            : Event(EVENT_EQUIP, sizeof(EquipEvent)),
        type(type) {
    }

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(EquipEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(EquipEvent));
    }

    inline unsigned int getEntityId() const {
        return entityId;
    }

    inline void setEntityId(unsigned int entityId) {
        this->entityId = entityId;
    }

    inline EntityType getType() const {
        return type;
    }

    inline void setType(EntityType type) {
        this->type = type;
    }

    friend std::ostream& operator<<(std::ostream &os, const EquipEvent &o) {
        os << "EquipEvent: {" << std::endl;
        os << "    entityId: " << o.entityId << std::endl;
        os << "    type: " << o.type << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int entityId;
    EntityType type;
};

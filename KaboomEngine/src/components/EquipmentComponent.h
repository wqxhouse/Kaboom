#pragma once

#include <ostream>

#include "Component.h"

enum EntityType;

class EquipmentComponent : public Component {
public:
    EquipmentComponent(EntityType type)
            : type(type) {
    }

    inline EntityType getType() const {
        return type;
    }

    inline void setType(EntityType type) {
        this->type = type;
    }

    friend std::ostream &operator<<(std::ostream &os, const EquipmentComponent &o) {
        os << "EquipmentComponent: {" << std::endl;
        os << "    type: " << o.type << std::endl;
        os << "}";

        return os;
    }

private:
    EntityType type;
};

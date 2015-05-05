#pragma once

#include <ostream>

#include "Component.h"

enum EntityType;

class EquipmentComponent : public Component {
public:
    EquipmentComponent(EntityType equipmentType);

    EntityType getEquipmentType() const;
    void setEquipmentType(EntityType equipmentType);

    friend std::ostream &operator<<(std::ostream &os, const EquipmentComponent &o) {
        os << "EquipmentComponent: {" << std::endl;
        os << "    equipmentType: " << o.equipmentType << std::endl;
        os << "}";

        return os;
    }

private:
    EntityType equipmentType;
};

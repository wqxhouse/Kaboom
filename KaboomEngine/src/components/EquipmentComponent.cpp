#include "EquipmentComponent.h"

EquipmentComponent::EquipmentComponent(EntityType equipmentType)
        : equipmentType(equipmentType) {
}

EntityType EquipmentComponent::getEquipmentType() const {
    return equipmentType;
}

void EquipmentComponent::setEquipmentType(EntityType equipmentType) {
    this->equipmentType = equipmentType;
}

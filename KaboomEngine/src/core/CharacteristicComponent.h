#pragma once

#include <ostream>

#include "Component.h"
#include "EntityType.h"

class CharacteristicComponent : public Component {
public:
    CharacteristicComponent();
    CharacteristicComponent(EntityType type, int feature, int owner);
    ~CharacteristicComponent();

    EntityType getType() const;
    void setType(EntityType type);

    int getFeature() const;
    void setFeature(int feature);

    int getOwner() const;
    void setOwner(int owner);

    friend std::ostream& operator<<(std::ostream &os, const CharacteristicComponent &o) {
        os << "CharacteristicComponent: {" << std::endl;
        os << "    type: " << o.type << std::endl;
        os << "    feature: " << o.feature << std::endl;
        os << "    owner: " << o.owner << std::endl;
        os << "}";
        return os;
    }

private:
    EntityType type;
    int feature;
    int owner;
};
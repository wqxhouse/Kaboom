#include "EntityConfigLookup.h"

#include "BombConfigLoader.h"
#include "CharacterConfigLoader.h"

const EntityConfigLookup& EntityConfigLookup::instance() {
    static bool initialized = false;
    static EntityConfigLookup instance;

    if (!initialized) {
        instance.load("data-server/bombs.xml");
        instance.load("data-server/characters.xml");

        initialized = true;
    }

    return instance;
}

void EntityConfigLookup::load(const std::string &filename) {
    BombConfigLoader bombConfigLoader(config);
    bombConfigLoader.load(filename);

    CharacterConfigLoader charConfigLoader(config);
    charConfigLoader.load(filename);
}

const Configuration &EntityConfigLookup::operator[](EntityType type) const {
    return config.at(type);
}

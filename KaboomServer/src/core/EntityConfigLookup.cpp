#include "EntityConfigLookup.h"

#include "BombDataLoader.h"

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
    BombDataLoader bombDataLoader(config);
    bombDataLoader.load(filename);
}

const Configuration &EntityConfigLookup::operator[](EntityType type) const {
    return config.at(type);
}

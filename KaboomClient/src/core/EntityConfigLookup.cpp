#include "EntityConfigLookup.h"

#include "BombConfigLoader.h"

const EntityConfigLookup &EntityConfigLookup::instance() {
    static bool initialized = false;
    static EntityConfigLookup instance;

    if (!initialized) {
        instance.load("data-client/bombs.xml");

        initialized = true;
    }

    return instance;
}

void EntityConfigLookup::load(const std::string &filename) {
    BombConfigLoader bombConfigLoader(config);
    bombConfigLoader.load(filename);
}

const Configuration &EntityConfigLookup::operator[](EntityType type) const {
    return config.at(type);
}

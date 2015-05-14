#include "EntityConfigLookup.h"

#include "BombConfigLoader.h"

bool EntityConfigLookup::initialized = false;
EntityConfigLookup EntityConfigLookup::instance;

const Configuration &EntityConfigLookup::get(EntityType type) {
    if (!initialized) {
        initialize();
    }

    return instance.config.at(type);
}

void EntityConfigLookup::initialize() {
    if (!initialized) {
        initialized = true;
        instance.loadBombConfig("data-client/bombs.xml");
    }
}

void EntityConfigLookup::loadBombConfig(const std::string &filename) {
    BombConfigLoader loader(config);
    loader.load(filename);
}

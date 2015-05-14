#include "EntityConfigLookup.h"

#include "BombConfigLoader.h"
#include "CharacterConfigLoader.h"

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
        instance.loadBombConfig("data-server/bombs.xml");
        instance.loadCharacterConfig("data-server/characters.xml");
    }
}

void EntityConfigLookup::loadBombConfig(const std::string &filename) {
    BombConfigLoader loader(config);
    loader.load(filename);
}

void EntityConfigLookup::loadCharacterConfig(const std::string &filename) {
    CharacterConfigLoader loader(config);
    loader.load(filename);
}

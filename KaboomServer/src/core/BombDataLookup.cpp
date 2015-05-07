#include "BombDataLookup.h"

#include "BombDataLoader.h"

const BombDataLookup& BombDataLookup::instance() {
    static bool initialized = false;
    static BombDataLookup instance;

    if (!initialized) {
        instance.load("data-server/bombs.xml");
    }

    return instance;
}

void BombDataLookup::load(const std::string &filename) {
    BombDataLoader loader(config);
    loader.load(filename);
}

const Configuration &BombDataLookup::operator[](EntityType type) const {
    return config.at(type);
}

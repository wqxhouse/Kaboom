#pragma once

#include <string>
#include <unordered_map>

#include <util/Configuration.h>

enum EntityType;

class BombDataLookup {
public:
    static const BombDataLookup& instance();

    void load(const std::string &filename);

    const Configuration &operator[](EntityType type) const;

private:
    BombDataLookup() {}

    std::unordered_map<EntityType, Configuration> config;
};

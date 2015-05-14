#pragma once

#include <string>
#include <unordered_map>

#include <util/Configuration.h>

enum EntityType;

class EntityConfigLookup {
public:
    static const EntityConfigLookup &instance();

    void load(const std::string &filename);

    const Configuration &operator[](EntityType type) const;

private:
    EntityConfigLookup() {}

    std::unordered_map<EntityType, Configuration> config;
};

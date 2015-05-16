#pragma once

#include <string>

class World;

class WorldLoader {
public:
    WorldLoader(World &world);

    void load(const std::string &filename, const std::string &mediaPath);

private:
    World &world;
};

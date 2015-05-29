#pragma once

#include <string>

class World;

class WorldLoader {
public:
    WorldLoader(World &world);

    void loadMap(const std::string &filename, const std::string &mediaPath);
    void loadEntities(const std::string &filename);

private:
    World &world;
};

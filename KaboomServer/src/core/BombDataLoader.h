#pragma once

#include <unordered_map>

#include <core/EntityType.h>
#include <util/Configuration.h>
#include <util/XMLLoader.h>

#include "BombData.h"

class BombDataLoader : public XMLLoader {
public:
    BombDataLoader(std::unordered_map<EntityType, Configuration> &config);

    void load(const std::string &filename);

private:
    std::unordered_map<EntityType, Configuration> &config;

    virtual void loadXMLNode(osgDB::XmlNode *xmlRoot);

    void loadValue(osgDB::XmlNode *xmlNode, const std::string &valueType, EntityType type);
};

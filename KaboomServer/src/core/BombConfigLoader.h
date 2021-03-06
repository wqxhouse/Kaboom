#pragma once

#include <unordered_map>

#include <util/XMLLoader.h>

class Configuration;
enum EntityType;

class BombConfigLoader : public XMLLoader {
public:
    BombConfigLoader(std::unordered_map<EntityType, Configuration> &config);

    void load(const std::string &filename);

private:
    std::unordered_map<EntityType, Configuration> &config;

    virtual void loadXMLNode(osgDB::XmlNode *xmlRoot);

    void loadValue(osgDB::XmlNode *xmlNode, const std::string &valueType, EntityType type);
};

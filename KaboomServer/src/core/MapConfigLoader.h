#pragma once

#include <unordered_map>

#include <util/XMLLoader.h>

class Configuration;
enum EntityType;

class MapConfigLoader : public XMLLoader {
public:
    typedef std::unordered_map<std::string, Configuration> SpawnPointNameToConfigMap;

    MapConfigLoader(SpawnPointNameToConfigMap &config);

    void load(const std::string &filename);

private:
    SpawnPointNameToConfigMap &config;

    int uniqueId;

    virtual void loadXMLNode(osgDB::XmlNode *xmlRoot);

	void loadValue(osgDB::XmlNode *xmlNode, const std::string &valueType, std::string id);
};

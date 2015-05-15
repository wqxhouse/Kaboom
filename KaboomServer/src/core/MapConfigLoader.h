#pragma once

#include <unordered_map>

#include <util/XMLLoader.h>

class Configuration;
enum EntityType;

class MapConfigLoader : public XMLLoader {
public:
	MapConfigLoader(std::unordered_map<std::string, Configuration> &config);

    void load(const std::string &filename);

private:
    std::unordered_map<std::string, Configuration> &config;

    int uniqueId;

    virtual void loadXMLNode(osgDB::XmlNode *xmlRoot);

	void loadValue(osgDB::XmlNode *xmlNode, const std::string &valueType, std::string id);
};

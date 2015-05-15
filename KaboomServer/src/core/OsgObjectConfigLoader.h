#pragma once

#include <unordered_map>

#include <util/XMLLoader.h>

class Configuration;

class OsgObjectConfigLoader : public XMLLoader {
public:
	OsgObjectConfigLoader(std::unordered_map<std::string, Configuration> &config);

    void load(const std::string &filename);

private:
	std::unordered_map<std::string, Configuration> &config;

    virtual void loadXMLNode(osgDB::XmlNode *xmlRoot);

	void loadOsgObjectModel(osgDB::XmlNode* xmlNode, const std::string &modelName);
    void loadValue(osgDB::XmlNode *xmlNode, const std::string &valueType, const std::string &modelName);
};

#pragma once

#include <unordered_map>

#include <util/XMLLoader.h>

class Configuration;

class GameModeConfigLoader : public XMLLoader {
public:
    GameModeConfigLoader(std::unordered_map<std::string, Configuration> &config);

    void load(const std::string &filename);

private:
    std::unordered_map<std::string, Configuration> &config;

    virtual void loadXMLNode(osgDB::XmlNode *xmlRoot);

    void loadValue(osgDB::XmlNode *xmlNode, const std::string &valueType, const std::string &name);
};

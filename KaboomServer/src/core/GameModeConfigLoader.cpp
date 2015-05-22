#include "GameModeConfigLoader.h"

#include <util/Configuration.h>

GameModeConfigLoader::GameModeConfigLoader(std::unordered_map<std::string, Configuration> &config)
    : config(config) {
}

void GameModeConfigLoader::load(const std::string &filename) {
    loadXMLFile(filename);
}

void GameModeConfigLoader::loadXMLNode(osgDB::XmlNode *xmlRoot) {
    if (xmlRoot->type == osgDB::XmlNode::ROOT) {
        for (auto child : xmlRoot->children) {
            if (child->name == "game-modes") {
                return loadXMLNode(child);
            }
        }

        return;
    }

    for (auto characterNode : xmlRoot->children) {
        if (characterNode->name != "game-mode") {
            continue;
        }

        std::string name;

        for (auto dataNode : characterNode->children) {
            const std::string &valueType = dataNode->properties["type"];

            if (dataNode->name == "name") {
                loadString(dataNode, name);
            }

            loadValue(dataNode, valueType, name);
        }
    }
}

void GameModeConfigLoader::loadValue(osgDB::XmlNode *xmlNode, const std::string &valueType, const std::string &name) {
    if (valueType == "int") {
        int val;
        loadInt(xmlNode, val);
        config[name].set(xmlNode->name.c_str(), val);
    } else if (valueType == "uint") {
        unsigned int val;
        loadUint(xmlNode, val);
        config[name].set(xmlNode->name.c_str(), val);
    } else if (valueType == "float") {
        float val;
        loadFloat(xmlNode, val);
        config[name].set(xmlNode->name.c_str(), val);
    } else if (valueType == "string") {
        std::string val;
        loadString(xmlNode, val);
        config[name].set(xmlNode->name.c_str(), val);
    }
}

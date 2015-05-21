#include "CharacterConfigLoader.h"

#include <core/EntityType.h>
#include <util/Configuration.h>

#include "common.h"

CharacterConfigLoader::CharacterConfigLoader(std::unordered_map<EntityType, Configuration> &config)
    : config(config) {
}

void CharacterConfigLoader::load(const std::string &filename) {
    loadXMLFile(filename);
}

void CharacterConfigLoader::loadXMLNode(osgDB::XmlNode *xmlRoot) {
    if (xmlRoot->type == osgDB::XmlNode::ROOT) {
        for (auto child : xmlRoot->children) {
            if (child->name == "characters") {
                return loadXMLNode(child);
            }
        }

        return;
    }

    for (auto characterNode : xmlRoot->children) {
        if (characterNode->name != "character") {
            continue;
        }

        EntityType type = NONE;

        for (auto dataNode : characterNode->children) {
            const std::string &valueType = dataNode->properties["type"];

            if (dataNode->name == "id") {
                unsigned int id;
                loadUint(dataNode, id);
                type = static_cast<EntityType>(id);
            }

            if (dataNode->name == "starting-inventory") {
                loadInventory(dataNode, type);
            } else {
                loadValue(dataNode, valueType, type);
            }
        }
    }
}

void CharacterConfigLoader::loadInventory(osgDB::XmlNode *xmlNode, EntityType type) {
    InventoryType *inventory = new InventoryType();

    for (auto itemNode : xmlNode->children) {
        EntityType itemType = NONE;
        int amount = 0;

        for (auto itemDataNode : itemNode->children) {
            if (itemDataNode->name == "id") {
                unsigned int itemId = 0;
                loadUint(itemDataNode, itemId);
                itemType = static_cast<EntityType>(itemId);
            } else if (itemDataNode->name == "amount") {
                loadInt(itemDataNode, amount);
            }
        }

        (*inventory)[itemType] = amount;
    }

    config[type].set("starting-inventory", inventory);
}

void CharacterConfigLoader::loadValue(osgDB::XmlNode *xmlNode, const std::string &valueType, EntityType type) {
    if (valueType == "int") {
        int val;
        loadInt(xmlNode, val);
        config[type].set(xmlNode->name.c_str(), val);
    } else if (valueType == "uint") {
        unsigned int val;
        loadUint(xmlNode, val);
        config[type].set(xmlNode->name.c_str(), val);
    } else if (valueType == "float") {
        float val;
        loadFloat(xmlNode, val);
        config[type].set(xmlNode->name.c_str(), val);
    } else if (valueType == "string") {
        std::string val;
        loadString(xmlNode, val);
        config[type].set(xmlNode->name.c_str(), val);
    }
}

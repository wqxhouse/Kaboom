#include "BombConfigLoader.h"

#include "KaboomV2CollisionHandler.h"
#include "TimeBombTimerHandler.h"

BombConfigLoader::BombConfigLoader(std::unordered_map<EntityType, Configuration> &config)
        : config(config) {
}

void BombConfigLoader::load(const std::string &filename) {
    loadXMLFile(filename);
}

void BombConfigLoader::loadXMLNode(osgDB::XmlNode *xmlRoot) {
    if (xmlRoot->type == osgDB::XmlNode::ROOT) {
        for (auto child : xmlRoot->children) {
            if (child->name == "bombs") {
                return loadXMLNode(child);
            }
        }

        return;
    }

    for (auto bombNode : xmlRoot->children) {
        if (bombNode->name != "bomb") {
            continue;
        }

        EntityType type = NONE;

        for (auto dataNode : bombNode->children) {
            const std::string &valueType = dataNode->properties["type"];

            if (dataNode->name == "id") {
                unsigned int id;
                loadUint(dataNode, id);
                type = static_cast<EntityType>(id);
            }

            loadValue(dataNode, valueType, type);
        }

        const std::unordered_map<EntityType, CollisionHandler *> collisionHandlers = {
            { KABOOM_V2, new KaboomV2CollisionHandler() }
        };

        if (collisionHandlers.count(type) > 0) {
            config[type].set("collision-handler", collisionHandlers.at(type));
        }

        const std::unordered_map<EntityType, TimerHandler *> timerHandlers = {
            { TIME_BOMB, new TimeBombTimerHandler() }
        };

        if (timerHandlers.count(type) > 0) {
            config[type].set("timer-handler", timerHandlers.at(type));
        }
    }
}

void BombConfigLoader::loadValue(osgDB::XmlNode *xmlNode, const std::string &valueType, EntityType type) {
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
        config[type].set(xmlNode->name.c_str(), val.c_str());
    }
}

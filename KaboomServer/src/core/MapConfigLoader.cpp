#include "MapConfigLoader.h"

#include <core/EntityType.h>
#include <util/Configuration.h>

MapConfigLoader::MapConfigLoader(SpawnPointNameToConfigMap &config)
        : config(config),
		  uniqueId(0) {
}

void MapConfigLoader::load(const std::string &filename){
    loadXMLFile(filename);
}

void MapConfigLoader::loadXMLNode(osgDB::XmlNode *xmlRoot) {
	if (xmlRoot->type == osgDB::XmlNode::ROOT) {
		for (auto child : xmlRoot->children) {
			if (child->name == "map") {
				return loadXMLNode(child);
			}
		}

		return;
	}

	for (auto spawnsNode : xmlRoot->children) {
		if (spawnsNode->name != "objects") {
			continue;
		}

		EntityType type = NONE;

		for (auto spawnNode : spawnsNode->children) {

			std::string id = spawnNode->name + std::to_string(uniqueId++);
			for (auto dataNode : spawnNode->children) {
				loadValue(dataNode, dataNode->properties["type"], id);
			}
		}
	}
}

void MapConfigLoader::loadValue(osgDB::XmlNode *xmlNode, const std::string &valueType, std::string id) {
    if (valueType == "int") {
        int val;
        loadInt(xmlNode, val);
		config[id].set(xmlNode->name.c_str(), val);
    } else if (valueType == "uint") {
        unsigned int val;
        loadUint(xmlNode, val);
		config[id].set(xmlNode->name.c_str(), val);
    } else if (valueType == "float") {
        float val;
        loadFloat(xmlNode, val);
		config[id].set(xmlNode->name.c_str(), val);
    } else if (valueType == "string") {
        std::string val;
        loadString(xmlNode, val);
		config[id].set(xmlNode->name.c_str(), val);
	} else if (valueType == "vector3") {
		osg::Vec3 val;
		loadVec3(xmlNode, val);
		config[id].set(xmlNode->name.c_str(), val);
	}

}

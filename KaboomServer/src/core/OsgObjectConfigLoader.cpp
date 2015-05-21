#include "OsgObjectConfigLoader.h"

#include <util/Configuration.h>

OsgObjectConfigLoader::OsgObjectConfigLoader(std::unordered_map<std::string, Configuration> &config)
        : config(config) {
}

void OsgObjectConfigLoader::load(const std::string &filename) {
    loadXMLFile(filename);
}

void OsgObjectConfigLoader::loadXMLNode(osgDB::XmlNode *xmlRoot) {
    if (xmlRoot->type == osgDB::XmlNode::ROOT) {
        for (auto child : xmlRoot->children) {
            if (child->name == "world") {
                return loadXMLNode(child);
            }
        }

        return;
    }

    for (auto modelNode : xmlRoot->children) {
		if (modelNode->name != "model") {
            continue;
        }
		const std::string name = modelNode->properties["name"];

		for (auto dataNode : modelNode->children) {
			if (!isXMLNodeType(dataNode)) {
				continue;
			}
			loadOsgObjectModel(dataNode, name);
        }
    }
}

void OsgObjectConfigLoader::loadOsgObjectModel(osgDB::XmlNode *xmlNode, const std::string &modelName) {
	const std::string typeName = xmlNode->name;

	if (typeName == "file") {
		loadValue(xmlNode, "string", modelName);
	} else if (typeName == "position") {
		loadValue(xmlNode, "vector3", modelName);
	} else if (typeName == "orientation") {
		loadValue(xmlNode, "vector4", modelName);
	} else if (typeName == "scale") {
		loadValue(xmlNode, "vector3", modelName);
	}
}


void OsgObjectConfigLoader::loadValue(osgDB::XmlNode *xmlNode, const std::string &valueType, const std::string &modelName) {
	if (valueType == "int") {
		int val;
		loadInt(xmlNode, val);
		config[modelName].set(xmlNode->name.c_str(), val);
	} else if (valueType == "uint") {
		unsigned int val;
		loadUint(xmlNode, val);
		config[modelName].set(xmlNode->name.c_str(), val);
	} else if (valueType == "float") {
		float val;
		loadFloat(xmlNode, val);
		config[modelName].set(xmlNode->name.c_str(), val);
	} else if (valueType == "string") {
		std::string val;
		loadString(xmlNode, val);
		config[modelName].set(xmlNode->name.c_str(), val);
	} else if (valueType == "vector3") {
		osg::Vec3 val;
		loadVec3(xmlNode, val);
		config[modelName].set(xmlNode->name.c_str(), val);
	} else if (valueType == "vector4") {
		osg::Vec4 val;
		loadVec4(xmlNode, val);
		config[modelName].set(xmlNode->name.c_str(), val);
	}
}
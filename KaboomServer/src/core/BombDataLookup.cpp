#include "BombDataLookup.h"

#include "KaboomV2CollisionHandler.h"

const BombDataLookup BombDataLookup::instance("data-server/bombs.xml");

BombDataLookup::BombDataLookup(const std::string &filename) {
    loadXMLFile(filename);
}

void BombDataLookup::loadXMLNode(osgDB::XmlNode *xmlRoot) {
    if (xmlRoot->type == osgDB::XmlNode::ROOT) {
        for (auto child : xmlRoot->children) {
            if (child->name == "bombs") {
                return loadXMLNode(child);
            }
        }

        return;
    }

    std::unordered_map<EntityType, CollisionHandler *> collisionHandlers = {
        { KABOOM_V2, new KaboomV2CollisionHandler() }
    };

    for (auto bombNode : xmlRoot->children) {
        if (bombNode->name != "bomb") {
            continue;
        }

        BombData data;

        for (auto dataNode : bombNode->children) {
            if (dataNode->name == "id") {
                unsigned int id;
                loadUint(dataNode, id);

                data.type = static_cast<EntityType>(id);
                data.collisionHandler = collisionHandlers[data.type];
            } else if (dataNode->name == "name") {
                loadString(dataNode, data.name);
            } else if (dataNode->name == "size") {
                loadFloat(dataNode, data.size);
            } else if (dataNode->name == "mass") {
                loadFloat(dataNode, data.mass);
            } else if (dataNode->name == "explosion-radius") {
                loadFloat(dataNode, data.explosionRadius);
            } else if (dataNode->name == "cooldown") {
                loadInt(dataNode, data.cooldown);
            } else if (dataNode->name == "launch-speed") {
                loadInt(dataNode, data.launchSpeed);
            } else if (dataNode->name == "capacity") {
                loadInt(dataNode, data.capacity);
			} else if (dataNode->name == "knockback-ratio") {
				loadFloat(dataNode, data.knockbackRatio);
			} else if (dataNode->name == "maxDamage") {
				loadInt(dataNode, data.maxDamage);
			} else if (dataNode->name == "minDamage") {
				loadInt(dataNode, data.minDamage);
			} else if (dataNode->name == "knockback-duration") {
				loadInt(dataNode, data.knockbackDuration);
			}
        }

        bombs[data.type] = data;
    }
}

const BombData &BombDataLookup::operator[](const EntityType &type) const {
    return bombs.at(type);
}

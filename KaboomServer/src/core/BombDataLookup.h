#pragma once

#include <unordered_map>

#include <core/EntityType.h>
#include <util/XMLLoader.h>

#include "BombData.h"

class BombDataLookup : public XMLLoader {
public:
    static const BombDataLookup instance;

    BombDataLookup(const std::string &filename);

    const BombData &operator[](const EntityType &type) const;

private:
    std::unordered_map<EntityType, BombData> bombs;

    virtual void loadXMLNode(osgDB::XmlNode *xmlRoot);
};

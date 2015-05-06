#pragma once

#include <unordered_map>

#include <core/EntityType.h>
#include <util/XMLLoader.h>

#include "CharacterData.h"

class CharacterDataLookup : public XMLLoader {
public:
    static const CharacterDataLookup instance;

    CharacterDataLookup(const std::string &filename);

    const CharacterData &operator[](const EntityType &type) const;

private:
    std::unordered_map<EntityType, CharacterData> characters;

    virtual void loadXMLNode(osgDB::XmlNode *xmlRoot);
};

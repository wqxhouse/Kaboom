#pragma once

#include <Windows.h>

#include <osg/Group>
#include <osg/MatrixTransform>

#include "PlayerData.h"
#include "OldEntity.h"

class Player : public OldEntity, public osg::Group {
public:
    Player(PlayerData *data);
    ~Player();

    virtual EntityData *getEntityData();
    void setPosition(float x, float y, float z);

protected:
    osg::ref_ptr<osg::MatrixTransform> positionTransform;
    PlayerData *data;
};

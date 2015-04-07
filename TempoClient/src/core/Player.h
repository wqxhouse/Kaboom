#pragma once

#include <Windows.h>

#include <osg/Group>
#include <osg/MatrixTransform>

#include "PlayerData.h"

class Player : public osg::Group {
public:
    PlayerData *data;

    Player(PlayerData *data);
    ~Player();

    void setPosition(const osg::Vec3 &position);

protected:
    osg::ref_ptr<osg::MatrixTransform> positionTransform;
};

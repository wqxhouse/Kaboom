#pragma once

#include <Windows.h>

#include <osg/Group>
#include <osg/MatrixTransform>

#include "Player.h"

class PlayerNode : public osg::Group {
public:
    PlayerNode(Player *player);
    ~PlayerNode();

    void setPosition(const osg::Vec3 &position);

protected:
    Player* player;

    osg::ref_ptr<osg::MatrixTransform> positionTransform;
};

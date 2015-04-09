#include "Player.h"

#include <iostream>

#include <osg/Geode>
#include <osg/ShapeDrawable>

Player::Player(PlayerData *data) : data(data), positionTransform(new osg::MatrixTransform) {
    osg::ref_ptr<osg::Box> box = new osg::Box;
    osg::ref_ptr<osg::ShapeDrawable> drawable = new osg::ShapeDrawable(box);

    osg::ref_ptr<osg::Geode> model = new osg::Geode;
    model->addDrawable(drawable);

    this->addChild(positionTransform);
    positionTransform->addChild(model);

    setPosition(data->x, data->y, data->z);
}

Player::~Player() {
}

EntityData *Player::getEntityData() {
    return data;
}

void Player::setPosition(float x, float y, float z) {
    this->data->x = x;
    this->data->y = y;
    this->data->z = z;

    osg::Matrix translationMat;
    translationMat.makeTranslate(osg::Vec3(x, y, z));

    positionTransform->setMatrix(translationMat);
}

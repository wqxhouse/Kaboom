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

    setPosition(data->position);
}

Player::~Player() {
}

void Player::setPosition(const osg::Vec3 &position) {
    this->data->position = position;

    osg::Matrix translationMat;
    translationMat.makeTranslate(position);

    positionTransform->setMatrix(translationMat);
}

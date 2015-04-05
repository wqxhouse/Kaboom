#include "PlayerNode.h"

#include <iostream>

#include <osg/Geode>
#include <osg/ShapeDrawable>

PlayerNode::PlayerNode(Player *player) : player(player), positionTransform(new osg::MatrixTransform) {
    osg::ref_ptr<osg::Box> box(new osg::Box);
    osg::ref_ptr<osg::ShapeDrawable> playerDrawable(new osg::ShapeDrawable(box));

    osg::ref_ptr<osg::Geode> model(new osg::Geode);
    model->addDrawable(playerDrawable);

    this->addChild(positionTransform);
    positionTransform->addChild(model);

    setPosition(player->position);
}

PlayerNode::~PlayerNode() {
}

void PlayerNode::setPosition(const osg::Vec3 &position) {
    osg::Matrix translationMat;
    translationMat.makeTranslate(position);

    positionTransform->setMatrix(translationMat);
}

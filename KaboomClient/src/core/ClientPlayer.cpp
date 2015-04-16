#include "ClientPlayer.h"

#include <Windows.h>

#include <osg/MatrixTransform>

#include "SceneNodeComponent.h"

ClientPlayer::ClientPlayer(unsigned int id)
    : Player(id) {
}

ClientPlayer::~ClientPlayer() {
}

void ClientPlayer::setPosition(float x, float y, float z) {
	setX(x);
	setY(y);
	setZ(z);
	
	osg::Group *rootNode = static_cast<osg::Group *>(getComponent<SceneNodeComponent>()->getNode());
	osg::MatrixTransform *transformNode = static_cast<osg::MatrixTransform *>(rootNode->getChild(0));
	
	osg::Matrix transformMat;
	transformMat.makeTranslate(x, y, z);
	
	transformNode->setMatrix(transformMat);
}
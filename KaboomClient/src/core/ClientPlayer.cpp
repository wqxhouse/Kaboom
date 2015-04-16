#include "ClientPlayer.h"

#include <Windows.h>

#include <osg/MatrixTransform>

#include "SceneNodeComponent.h"

ClientPlayer::ClientPlayer(unsigned int id)
    : Player(id) {
}

ClientPlayer::~ClientPlayer() {
}

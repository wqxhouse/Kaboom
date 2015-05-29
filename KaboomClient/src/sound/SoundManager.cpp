#include "SoundManager.h"

#include <osgAudio/Sample.h>
#include <osgAudio/SoundManager.h>

SoundManager::SoundManager() {
    osgAudio::SoundManager::instance()->init(16); // HACK: Hard-coded constant
}

SoundManager::~SoundManager() {
    osgAudio::SoundManager::instance()->shutdown();
}

void SoundManager::loadSound(SoundType type, const std::string &filename) {
    const auto name = getSoundName(type);

    osgAudio::Sample *sample = new osgAudio::Sample(filename);

    osg::ref_ptr<osgAudio::SoundState> soundState = new osgAudio::SoundState(name);
    soundState->setSample(sample);
    soundState->setGain(0.7f);
    soundState->setReferenceDistance(10);
    soundState->setPlay(false);
    soundState->setLooping(false);

    osgAudio::SoundManager::instance()->addSoundState(soundState.get());
}

void SoundManager::playSound(SoundType type) {
    const auto listener = osgAudio::SoundManager::instance()->getListener();
    osg::Vec3 listenerPos;
    listener->getPosition(listenerPos.x(), listenerPos.y(), listenerPos.z());

    auto sound = getSound(type);
    sound->setPlay(true);
    sound->setPosition(listenerPos);
    osgAudio::SoundManager::instance()->pushSoundEvent(sound);
}

void SoundManager::playSound(SoundType type, const Vec3 &position) {
    auto sound = getSound(type);
    sound->setPlay(true);
    sound->setPosition(position.getOsgVec3());
    osgAudio::SoundManager::instance()->pushSoundEvent(sound);
}

void SoundManager::setListenerPosition(const Vec3 &position) const {
    const auto x = position.x;
    const auto y = position.y;
    const auto z = position.z;
    const auto listener = osgAudio::SoundManager::instance()->getListener();
    listener->setPosition(x, y, z);
}

void SoundManager::setListenerRotation(const Quat &rotation) const {
    osg::Matrix orientation;
    osg::Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
    rot.get(orientation);

    osg::Vec3 dir = orientation.preMult(osg::Vec3(0.0f, 1.0f, 0.0f));

    const auto listener = osgAudio::SoundManager::instance()->getListener();
    listener->setOrientation(dir.x(), dir.y(), dir.z(), 0.0f, 0.0f, 1.0f);
}

osgAudio::SoundState *SoundManager::getSound(SoundType type) {
    const auto name = getSoundName(type);
    return osgAudio::SoundManager::instance()->findSoundState(name);
}

std::string SoundManager::getSoundName(SoundType type) const {
    return std::to_string(static_cast<unsigned int>(type));
}

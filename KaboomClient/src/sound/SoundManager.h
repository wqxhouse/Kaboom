#pragma once

#include <string>

#include <osgAudio/SoundState.h>

#include <math/Vec3.h>
#include <math/Quat.h>

enum class SoundType {
    ROBOTS,
    BASIC,
    KABOOM_FIRE,
    KABOOM_EXPLODE
};

class SoundManager {
public:
    SoundManager();
    ~SoundManager();

    void loadSound(SoundType type, const std::string &filename);

    void playSound(SoundType type);
    void playSound(SoundType type, const Vec3 &position);

    void setListenerPosition(const Vec3 &position) const;
    void setListenerRotation(const Quat &rotation) const;

private:
    osgAudio::SoundState *getSound(SoundType type);
    std::string getSoundName(SoundType type) const;
};

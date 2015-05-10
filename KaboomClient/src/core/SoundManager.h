#pragma once

#include <unordered_map>

#include <string>
#include <osgAudio/Source.h>
#include <osgAudio/AudioEnvironment.h>
#include <osgAudio/Sample.h>

using namespace osgAudio;
enum SOUNDS{
	BASIC,
	KABOOM_FIRE,
	KABOOM_EXPLODE

};

class SoundManager {
public:
    SoundManager();
	~SoundManager();
	void addToMap(SOUNDS s, std::string location);
	osg::ref_ptr<Sample> returnSound(SOUNDS s);
	void changeBackground(SOUNDS s);

    

private:
	std::unordered_map<SOUNDS, osg::ref_ptr<Sample> > *sounds;
	osg::ref_ptr<Source> source;
};

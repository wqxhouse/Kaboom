#include "SoundManager.h"

#include <iostream>
#include <sstream>

SoundManager::SoundManager(){
	sounds = new std::unordered_map<SOUNDS, osg::ref_ptr<Sample> >();
}
SoundManager::~SoundManager(){

}
void SoundManager::addToMap(SOUNDS s, std::string location){
	osg::ref_ptr<Sample> sample = new Sample(location);
	sounds->insert(std::make_pair(s, sample));

}
osg::ref_ptr<Sample> SoundManager::returnSound(SOUNDS s){
	return sounds->at(s);	
}
void SoundManager::changeBackground(SOUNDS s){
	source = new Source;
	source->setSound(sounds->at(s));
	source->setGain(1);
	source->setLooping(true);
	source->play();
}


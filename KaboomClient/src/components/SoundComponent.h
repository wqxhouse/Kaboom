#pragma once

#include <Model.h>
#include <osgAudio/Source.h>
//#include <osgAudio/AudioEnvironment.h>
using namespace osgAudio;
#include <components/Component.h>
#include <osgAudio/SoundState.h>
#include <osgAudio/Sample.h>
#include <osgAudio/SoundManager.h>
#include <math/Vec3.h>
#include <math/Quat.h>


class SoundComponent : public Component {
public:
	SoundComponent(std::string name,osg::ref_ptr<Sample> walk) {
		//osgAudio::SoundState *s = osgAudio::SoundManager::instance()->findSoundState(name);
		walkingSound = new osgAudio::SoundState("walker");
		//const osgAudio::Sample *d = s->getSample();
		//source->setSound();
		//walkingSound->setSample(d);
		osg::ref_ptr<osgAudio::Sample> sample = walk;
		walkingSound->setPlay(false);
		walkingSound->setLooping(true);
		walkingSound->setSample(sample.get());
		source = new Source;
	
		walkingSound->setSource(source);
		//walkingSound->setPosition();
		//osgAudio::SoundManager::instance()->pushSoundEvent(walkingSound);
		//osgAudio::SoundManager::instance()->setUpdateFrequency(1);
		//printf("hello");
		playing = false;
	}

	inline void playSound( const Vec3 &position) {
		/*if (walkingSound->isPlaying()){
			//osgAudio::SoundManager::instance()->removeSoundState(walkingSound);
			walkingSound->setPosition(position.getOsgVec3());
			osgAudio::SoundManager::instance()->update();
			return;
		}*/
		//source = new Source;
		//walkingSound->setSource(source);
		auto sou = walkingSound;
		sou->setPlay(true);
		//sou->setLooping(true);
		sou->setPosition(position.getOsgVec3());
		
		osgAudio::SoundManager::instance()->update();
		
	}
	inline void stopSound() {
		auto sou = walkingSound;
		sou->setPlay(false);
		osgAudio::SoundManager::instance()->update();
		//osgAudio::SoundManager::instance()->pushSoundEvent(sou);
	}
	inline void setListenerPosition(const Vec3 &position) const {
		const auto x = position.x;
		const auto y = position.y;
		const auto z = position.z;
		const auto listener = osgAudio::SoundManager::instance()->getListener();
		osgAudio::SoundManager::instance()->update();
		listener->setPosition(x, y, z);
	}

	inline void setListenerRotation(const Quat &rotation) const {
		osg::Matrix orientation;
		osg::Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
		rot.get(orientation);

		osg::Vec3 dir = orientation.preMult(osg::Vec3(0.0f, 1.0f, 0.0f));

		const auto listener = osgAudio::SoundManager::instance()->getListener();
		listener->setOrientation(dir.x(), dir.y(), dir.z(), 0.0f, 0.0f, 1.0f);
	}

private:
	osgAudio::SoundState *walkingSound;
	bool playing;
	osg::ref_ptr<Source> source;
	
};

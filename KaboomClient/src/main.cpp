#include <util/ConfigSettings.h>

#include "core/Game.h"

int main() {
    ConfigSettings *config = ConfigSettings::config;

    Game game(config);
    game.run();
}

/* -*-c++-*- */
/*Skip to content
Sign up Sign in This repository
Explore
Features
Enterprise
Blog
Watch 4  Star 2  Fork 1 mccdo / osgaudio
branch : master  osgaudio / examples / osgaudio - lowlevel / multiple.cpp
SkewMatrix@gmail.com on Jan 13, 2012 Copyright bump.
0 contributors
RawBlameHistory     78 lines(66 sloc)  2.519 kb
/* -*-c++-*- */
/**
* osgAudio - OpenSceneGraph Audio Library
* (C) Copyright 2009-2012 byKenneth Mark Bryden
* (programming by Chris 'Xenon' Hanson, AlphaPixel, LLC xenon at alphapixel.com)
* based on a fork of:
* Osg AL - OpenSceneGraph Audio Library
* Copyright (C) 2004 VRlab, Ume?University
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
* Please see COPYING file for special static-link exemption to LGPL.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
*/
/*
#include <osgAudio/Source.h>
#include <osgAudio/AudioEnvironment.h>
#include <osgAudio/Sample.h>

#include <iostream>
#include <string>
#include <vector>

using namespace osgAudio;

// usleep stub
#if defined(WIN32) && !defined (OPENALPP_WINDOWSSTUFF_H)
#include <windows.h>
inline void usleep(int x) { Sleep(x / 1000); };
#endif // defined(WIN32) && !defined (OPENALPP_WINDOWSSTUFF_H)

int main()
{
	osg::ref_ptr<Sample> sample;

	std::cerr << "Loads a sample at the time, plays it for a while, then loads another one etc.." << std::endl;
	try {
		osgAudio::AudioEnvironment::instance()->init();

		std::vector<std::string> file_vector;

		file_vector.push_back("C:\\Users\\melapier\\Downloads\\ins\\share\\osgAudio\\data\\a.wav");
		file_vector.push_back("C:\\Users\\melapier\\Downloads\\ins\\share\\osgAudio\\data\\high-e.wav");
		file_vector.push_back("C:\\Users\\melapier\\Downloads\\ins\\share\\osgAudio\\data\\low-e.wav");

		osg::ref_ptr<Source> source = new Source;
		unsigned int delay = 500;
		for (int i = 0; i < 10; i++) {
			if (sample.valid()) {
				source->stop();
			}
			std::string file = file_vector[i % file_vector.size()];
			std::cerr << "Loading and playing " << file << " for " << delay / 1000.0 << " seconds" << std::endl;
			sample = new Sample(file);
			std::cerr << "past the sample" << std::endl;
			source->setSound(sample.get());
			std::cerr << "past the setSound" << std::endl;
			source->setGain(1);
			std::cerr << "past the gain" << std::endl;
			source->setLooping();
			std::cerr << "past the looping" << std::endl;
			source->play();
			std::cerr << "past the play" << std::endl;
			usleep(delay * 1000); // Wait for delay milliseconds
		}
	}
	catch (Error e) {
		std::cerr << e << "\n";
	}

	return 0;
}*/

//Status API Training Shop Blog About
//?2015 GitHub, Inc.Terms Privacy Security Contact

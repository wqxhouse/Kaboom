#include <iostream>

#include <ConfigSettings.h>

#include "core/Game.h"

int main()
{
	ConfigSettings *config = ConfigSettings::config;
	Game game(config);

	try {
		game.run();
	}
	catch (std::exception &e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
}

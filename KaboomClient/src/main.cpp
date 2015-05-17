#include <util/ConfigSettings.h>

#include "core/Game.h"

int main() {
	ConfigSettings *config = ConfigSettings::config;

	Game game(config);
	game.run();
}

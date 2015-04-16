#include <iostream>

#include <Windows.h>

#include "network/GameClient.h"
#include "util/ConfigSettings.h"

#include "core/Game.h"

GameClient *g_client;

int main() {
    ConfigSettings* config = ConfigSettings::config;

    Game game(config);

    g_client = new GameClient(config, game.getEventHandlerLookup());

    try {
        game.run();
    } catch (std::exception &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }

    delete g_client;
}

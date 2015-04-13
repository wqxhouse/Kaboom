#include <ctime>

#include "core/Game.h"
#include "network/GameServer.h"

int main() {
    ConfigSettings* config = ConfigSettings::config;

    GameServer server(config);

    const clock_t FPS = 25;
    const clock_t TICK = 1000 / FPS;

    while (true) {
        clock_t beginTime = clock();

        server.update();

        clock_t endTime = clock();
        clock_t elapsedTime = endTime - beginTime;
        clock_t sleepTime = TICK - elapsedTime;

        if (sleepTime > 0) {
            Sleep(sleepTime);
        }
    }
}
#include <ctime>

#include "core/Game.h"

int main() {
    ConfigSettings* config = ConfigSettings::config;

    Game game(config);
    game.loadMap();

    const clock_t FPS = 60;
    const clock_t TICK = 1000 / FPS;

    while (true) {
        clock_t beginTime = clock();

        game.update(static_cast<float>(FPS));

        clock_t endTime = clock();
        clock_t elapsedTime = endTime - beginTime;
        clock_t sleepTime = TICK - elapsedTime;

        if (sleepTime > 0) {
            Sleep(sleepTime);
        }
    }
}
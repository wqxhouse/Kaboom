#include <ctime>

#include "core/Game.h"

int main() {
    ConfigSettings* config = ConfigSettings::config;

    Game game(config);

    const clock_t FPS = 60;
    const clock_t TICK = 1000 / FPS;
    const float TIME_STEP = 1.0f / FPS;
    const int MAX_SUB_STEPS = 1;

    while (true) {
        clock_t beginTime = clock();

        game.update(TIME_STEP, MAX_SUB_STEPS);

        clock_t endTime = clock();
        clock_t elapsedTime = endTime - beginTime;
        clock_t sleepTime = TICK - elapsedTime;

        if (sleepTime > 0) {
            Sleep(sleepTime);
        }
    }
}
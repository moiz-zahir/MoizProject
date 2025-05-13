#include "Timer.h"
#include "raylib.h"

Timer::Timer(int durationSeconds)
    : secondsLeft(durationSeconds), lastUpdateTime(GetTime()) {}

void Timer::update() {
    float currentTime = GetTime();
    if (currentTime - lastUpdateTime >= 1.0f && secondsLeft > 0) {
        secondsLeft--;
        lastUpdateTime = currentTime;
    }
}

int Timer::getSecondsLeft() const {
    return secondsLeft;
}
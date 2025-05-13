#pragma once

class Timer {
private:
    int secondsLeft;
    float lastUpdateTime;

public:
    Timer(int durationSeconds);
    void update();
    int getSecondsLeft() const;
};
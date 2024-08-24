#pragma once
#include <chrono>

class Timer {
public:
    void start();
    void stop();
    double elapsedMilliseconds() const;
    double elapsedSeconds() const;

    int elapsedTensOfMinutes() const;
    int elapsedMinutesOnly() const;
    int elapsedTensOfSeconds() const;
    int elapsedSecondsOnly() const;

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
    bool running;
};

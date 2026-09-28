#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
public:
    void start() {
        startTime = std::chrono::high_resolution_clock::now();
    }

    double elapsedMs() const {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = now - startTime;
        return elapsed.count();
    }

private:
    std::chrono::high_resolution_clock::time_point startTime;
};

#endif
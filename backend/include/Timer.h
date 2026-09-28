#ifndef TIMER_H
#define TIMER_H

#include <chrono>

// A tiny stopwatch utility. Deliberately kept OUTSIDE BFS/Dijkstra/AStar
// (separation of concerns) - the caller wraps a solve() call with this
// to measure wall-clock time, rather than the algorithms timing themselves.
//
// Uses steady_clock, NOT high_resolution_clock: on Windows/MinGW,
// high_resolution_clock can be a coarse system clock (1-15 ms ticks), which
// makes fast operations randomly report 0 ms. steady_clock uses the
// high-resolution performance counter and never goes backwards.
class Timer {
public:
    void start() {
        startTime = std::chrono::steady_clock::now();
    }

    double elapsedMs() const {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> elapsed = now - startTime;
        return elapsed.count();
    }

private:
    std::chrono::steady_clock::time_point startTime;
};

#endif

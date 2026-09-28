#include "Timer.h"
#include "Grid.h"
#include "BFS.h"
#include "Dijkstra.h"
#include "AStar.h"
#include <iostream>
#include <cassert>

int main() {
    // Test 1: elapsedMs should be roughly 0 right after start().
    {
        Timer t;
        t.start();
        double elapsed = t.elapsedMs();
        assert(elapsed >= 0.0);
        assert(elapsed < 50.0);
    }

    // Test 2: busy-wait proves Timer measures real elapsed time.
    {
        Timer t;
        t.start();
        volatile long busyCounter = 0;
        while (t.elapsedMs() < 50.0) {
            busyCounter++;
        }
        double elapsed = t.elapsedMs();
        assert(elapsed >= 50.0);
        assert(elapsed < 500.0);
    }

    // Test 3: elapsedMs should be monotonically non-decreasing.
    {
        Timer t;
        t.start();
        double first = t.elapsedMs();
        double second = t.elapsedMs();
        assert(second >= first);
    }

    // Test 4: integration - wrap real BFS/Dijkstra/A* calls with the Timer,
    // exactly like the future HTTP layer will, and confirm timeMs gets
    // filled in sensibly (non-negative, and actually recorded) for all three.
    {
        Grid g(30, 30);
        auto start = std::make_pair(0, 0);
        auto end = std::make_pair(29, 29);

        Timer t0;
        t0.start();
        PathResult bfsResult = BFS::solve(g, start, end);
        bfsResult.timeMs = t0.elapsedMs();

        Timer t1;
        t1.start();
        PathResult dijkstraResult = Dijkstra::solve(g, start, end);
        dijkstraResult.timeMs = t1.elapsedMs();

        Timer t2;
        t2.start();
        PathResult astarResult = AStar::solve(g, start, end);
        astarResult.timeMs = t2.elapsedMs();

        assert(bfsResult.timeMs >= 0.0);
        assert(dijkstraResult.timeMs >= 0.0);
        assert(astarResult.timeMs >= 0.0);

        std::cout << "  BFS time:      " << bfsResult.timeMs << " ms ("
                  << bfsResult.nodesExpanded << " nodes)" << std::endl;
        std::cout << "  Dijkstra time: " << dijkstraResult.timeMs << " ms ("
                  << dijkstraResult.nodesExpanded << " nodes)" << std::endl;
        std::cout << "  A* time:       " << astarResult.timeMs << " ms ("
                  << astarResult.nodesExpanded << " nodes)" << std::endl;
    }

    std::cout << "All Timer tests passed!" << std::endl;
    return 0;
}
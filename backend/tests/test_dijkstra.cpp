#include "Grid.h"
#include "BFS.h"
#include "Dijkstra.h"
#include <iostream>
#include <cassert>

// Helper: total cost of walking a path on a given grid (sum of cost to
// enter each cell after the first).
int pathCost(const Grid& g, const std::vector<std::pair<int,int>>& path) {
    int total = 0;
    for (size_t i = 1; i < path.size(); ++i) {
        total += g.getCost(path[i].first, path[i].second);
    }
    return total;
}

int main() {
    // Test 1: on an UNWEIGHTED grid with obstacles, Dijkstra should find
    // the exact same path length as BFS (cross-validation between algorithms).
    {
        Grid g(3, 3);
        g.setCell(1, 0, -1);
        g.setCell(1, 1, -1);
        PathResult bfsResult = BFS::solve(g, {0, 0}, {2, 0});
        PathResult dijkstraResult = Dijkstra::solve(g, {0, 0}, {2, 0});

        assert(bfsResult.found && dijkstraResult.found);
        assert(bfsResult.path.size() == dijkstraResult.path.size());
        assert(pathCost(g, bfsResult.path) == pathCost(g, dijkstraResult.path));
    }

    // Test 2: THE key test. A short route through expensive "mud" vs a
    // longer route around it. Dijkstra must pick the cheaper route, even
    // though it visits more cells.
    //
    // Grid (5 = mud, cost 5 to enter):
    //   S 5 5 .  .
    //   . 5 5 .  .
    //   . .  .  . E
    {
        Grid g(3, 5);
        g.setCell(0, 1, 5);
        g.setCell(0, 2, 5);
        g.setCell(1, 1, 5);
        g.setCell(1, 2, 5);

        auto start = std::make_pair(0, 0);
        auto end = std::make_pair(2, 4);

        PathResult result = Dijkstra::solve(g, start, end);
        assert(result.found);

        int cost = pathCost(g, result.path);

        // Around-the-bottom route is much cheaper than plowing through mud.
        assert(cost < 10);

        // Confirm the path does NOT pass through any mud cell.
        bool wentThroughMud = false;
        for (auto& cell : result.path) {
            if (g.getCost(cell.first, cell.second) == 5) wentThroughMud = true;
        }
        assert(!wentThroughMud);
    }

    // Test 3: unreachable target -> found should be false.
    {
        Grid g(3, 3);
        g.setCell(1, 0, -1);
        g.setCell(1, 1, -1);
        g.setCell(1, 2, -1);
        PathResult result = Dijkstra::solve(g, {0, 0}, {2, 0});
        assert(!result.found);
        assert(result.path.empty());
    }

    // Test 4: start == end.
    {
        Grid g(4, 4);
        PathResult result = Dijkstra::solve(g, {1, 1}, {1, 1});
        assert(result.found);
        assert(result.path.size() == 1);
    }

    // Test 5: start or end is a wall -> should fail gracefully.
    {
        Grid g(4, 4);
        g.setCell(3, 3, -1);
        PathResult result = Dijkstra::solve(g, {0, 0}, {3, 3});
        assert(!result.found);
    }

    std::cout << "All Dijkstra tests passed!" << std::endl;
    return 0;
}
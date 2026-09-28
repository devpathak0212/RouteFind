#include "Grid.h"
#include "BFS.h"
#include "Dijkstra.h"
#include "AStar.h"
#include <iostream>
#include <cassert>

int pathCost(const Grid& g, const std::vector<std::pair<int,int>>& path) {
    int total = 0;
    for (size_t i = 1; i < path.size(); ++i) {
        total += g.getCost(path[i].first, path[i].second);
    }
    return total;
}

int main() {
    // Test 1: on a grid with obstacles, A* must find the exact same
    // optimal cost as Dijkstra (correctness is non-negotiable).
    {
        Grid g(3, 5);
        g.setCell(0, 1, 5);
        g.setCell(0, 2, 5);
        g.setCell(1, 1, 5);
        g.setCell(1, 2, 5);

        auto start = std::make_pair(0, 0);
        auto end = std::make_pair(2, 4);

        PathResult dijkstraResult = Dijkstra::solve(g, start, end);
        PathResult astarResult = AStar::solve(g, start, end);

        assert(dijkstraResult.found && astarResult.found);
        assert(pathCost(g, dijkstraResult.path) == pathCost(g, astarResult.path));
    }

    // Test 2: THE key benchmark test. On a large open grid, A* should
    // expand fewer (or equal) nodes than Dijkstra, since the heuristic
    // steers it toward the goal instead of exploring in all directions
    // equally. This is the actual payoff of the whole project.
    {
        Grid g(30, 30); // large open grid, no obstacles
        auto start = std::make_pair(0, 0);
        auto end = std::make_pair(29, 29); // far corner

        PathResult dijkstraResult = Dijkstra::solve(g, start, end);
        PathResult astarResult = AStar::solve(g, start, end);

        assert(dijkstraResult.found && astarResult.found);
        // Same optimal path cost...
        assert(pathCost(g, dijkstraResult.path) == pathCost(g, astarResult.path));
        // ...but A* should expand meaningfully fewer nodes.
        assert(astarResult.nodesExpanded <= dijkstraResult.nodesExpanded);
        assert(astarResult.nodesExpanded < dijkstraResult.nodesExpanded); // strictly fewer

        std::cout << "  Dijkstra nodes expanded: " << dijkstraResult.nodesExpanded << std::endl;
        std::cout << "  A* nodes expanded:       " << astarResult.nodesExpanded << std::endl;
    }

    // Test 3: unreachable target.
    {
        Grid g(3, 3);
        g.setCell(1, 0, -1);
        g.setCell(1, 1, -1);
        g.setCell(1, 2, -1);
        PathResult result = AStar::solve(g, {0, 0}, {2, 0});
        assert(!result.found);
        assert(result.path.empty());
    }

    // Test 4: start == end.
    {
        Grid g(4, 4);
        PathResult result = AStar::solve(g, {1, 1}, {1, 1});
        assert(result.found);
        assert(result.path.size() == 1);
    }

    // Test 5: start or end is a wall.
    {
        Grid g(4, 4);
        g.setCell(3, 3, -1);
        PathResult result = AStar::solve(g, {0, 0}, {3, 3});
        assert(!result.found);
    }

    std::cout << "All A* tests passed!" << std::endl;
    return 0;
}
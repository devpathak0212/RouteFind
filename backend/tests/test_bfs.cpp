#include "Grid.h"
#include "BFS.h"
#include <iostream>
#include <cassert>

int main() {
    // Test 1: simple open grid, no obstacles.
    // Shortest path from (0,0) to (0,4) on an open row should be 5 steps long.
    {
        Grid g(5, 5);
        PathResult r = BFS::solve(g, {0, 0}, {0, 4});
        assert(r.found);
        assert(r.path.size() == 5); // (0,0)(0,1)(0,2)(0,3)(0,4)
        assert(r.path.front() == std::make_pair(0, 0));
        assert(r.path.back() == std::make_pair(0, 4));
    }

    // Test 2: a wall forces a detour.
    // Grid:
    //   . . .
    //   # # .
    //   . . .
    // Start (0,0), End (2,0). Direct column is blocked at row 1, col 0 and col 1,
    // so the only way through is around the right side via col 2.
    {
        Grid g(3, 3);
        g.setCell(1, 0, -1);
        g.setCell(1, 1, -1);
        PathResult r = BFS::solve(g, {0, 0}, {2, 0});
        assert(r.found);
        // Shortest detour: (0,0)->(0,1)->(0,2)->(1,2)->(2,2)->(2,1)->(2,0) = 7 cells
        assert(r.path.size() == 7);
        assert(r.path.front() == std::make_pair(0, 0));
        assert(r.path.back() == std::make_pair(2, 0));
    }

    // Test 3: completely walled off -> no path should be found.
    {
        Grid g(3, 3);
        g.setCell(1, 0, -1);
        g.setCell(1, 1, -1);
        g.setCell(1, 2, -1);
        PathResult r = BFS::solve(g, {0, 0}, {2, 0});
        assert(!r.found);
        assert(r.path.empty());
    }

    // Test 4: start == end should return immediately with a 1-cell path.
    {
        Grid g(4, 4);
        PathResult r = BFS::solve(g, {2, 2}, {2, 2});
        assert(r.found);
        assert(r.path.size() == 1);
        assert(r.nodesExpanded == 1);
    }

    // Test 5: start or end cell itself is a wall -> should fail gracefully.
    {
        Grid g(4, 4);
        g.setCell(3, 3, -1);
        PathResult r = BFS::solve(g, {0, 0}, {3, 3});
        assert(!r.found);
    }

    std::cout << "All BFS tests passed!" << std::endl;
    return 0;
}
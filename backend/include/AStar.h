#ifndef ASTAR_H
#define ASTAR_H

#include "Grid.h"
#include "BFS.h"
#include <utility>

class AStar {
public:
    static PathResult solve(const Grid& grid,
                             std::pair<int, int> start,
                             std::pair<int, int> end);

private:
    static int heuristic(std::pair<int, int> a, std::pair<int, int> b);
};

#endif
#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "Grid.h"
#include "BFS.h"
#include <utility>

class Dijkstra {
public:
    static PathResult solve(const Grid& grid,
                             std::pair<int, int> start,
                             std::pair<int, int> end);
};

#endif
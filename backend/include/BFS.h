#ifndef BFS_H
#define BFS_H

#include "Grid.h"
#include <vector>
#include <utility>

struct PathResult {
    std::vector<std::pair<int, int>> path;
    int nodesExpanded;
    bool found;
    double timeMs = 0.0; // filled in by the caller using Timer, not by the algorithm itself
};

class BFS {
public:
    static PathResult solve(const Grid& grid,
                             std::pair<int, int> start,
                             std::pair<int, int> end);
};

#endif
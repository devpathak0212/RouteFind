#include "BFS.h"
#include <queue>
#include <algorithm>

PathResult BFS::solve(const Grid& grid,
                       std::pair<int, int> start,
                       std::pair<int, int> end) {
    int rows = grid.getRows();
    int cols = grid.getCols();

    std::vector<bool> visited(rows * cols, false);
    std::vector<std::pair<int, int>> cameFrom(rows * cols, {-1, -1});

    auto toIndex = [cols](std::pair<int, int> cell) {
        return cell.first * cols + cell.second;
    };

    PathResult result;
    result.nodesExpanded = 0;
    result.found = false;

    if (!grid.isWalkable(start.first, start.second) ||
        !grid.isWalkable(end.first, end.second)) {
        return result;
    }

    std::queue<std::pair<int, int>> q;
    q.push(start);
    visited[toIndex(start)] = true;

    while (!q.empty()) {
        std::pair<int, int> current = q.front();
        q.pop();
        result.nodesExpanded++;

        if (current == end) {
            result.found = true;
            std::vector<std::pair<int, int>> path;
            std::pair<int, int> step = end;
            while (step != start) {
                path.push_back(step);
                step = cameFrom[toIndex(step)];
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            result.path = path;
            return result;
        }

        for (const auto& neighbor : grid.getNeighbors(current.first, current.second)) {
            int nIdx = toIndex(neighbor);
            if (!visited[nIdx]) {
                visited[nIdx] = true;
                cameFrom[nIdx] = current;
                q.push(neighbor);
            }
        }
    }

    return result;
}
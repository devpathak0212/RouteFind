#include "AStar.h"
#include "MinHeap.h"
#include <vector>
#include <algorithm>
#include <climits>
#include <cstdlib>

int AStar::heuristic(std::pair<int, int> a, std::pair<int, int> b) {
    return std::abs(a.first - b.first) + std::abs(a.second - b.second);
}

PathResult AStar::solve(const Grid& grid,
                         std::pair<int, int> start,
                         std::pair<int, int> end) {
    int rows = grid.getRows();
    int cols = grid.getCols();

    auto toIndex = [cols](std::pair<int, int> cell) {
        return cell.first * cols + cell.second;
    };
    auto toCell = [cols](int idx) {
        return std::make_pair(idx / cols, idx % cols);
    };

    std::vector<int> costSoFar(rows * cols, INT_MAX);
    std::vector<std::pair<int, int>> cameFrom(rows * cols, {-1, -1});
    std::vector<bool> finalized(rows * cols, false);

    PathResult result;
    result.nodesExpanded = 0;
    result.found = false;

    if (!grid.isWalkable(start.first, start.second) ||
        !grid.isWalkable(end.first, end.second)) {
        return result;
    }

    MinHeap heap;
    int startIdx = toIndex(start);
    costSoFar[startIdx] = 0;
    heap.push(heuristic(start, end), startIdx);

    while (!heap.empty()) {
        auto top = heap.pop();
        int currentIdx = top.second;

        if (finalized[currentIdx]) continue;

        finalized[currentIdx] = true;
        result.nodesExpanded++;

        std::pair<int, int> current = toCell(currentIdx);

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

        int currentTrueCost = costSoFar[currentIdx];

        for (const auto& neighbor : grid.getNeighbors(current.first, current.second)) {
            int nIdx = toIndex(neighbor);
            if (finalized[nIdx]) continue;

            int edgeCost = grid.getCost(neighbor.first, neighbor.second);
            int newTrueCost = currentTrueCost + edgeCost;

            if (newTrueCost < costSoFar[nIdx]) {
                costSoFar[nIdx] = newTrueCost;
                cameFrom[nIdx] = current;
                int priority = newTrueCost + heuristic(neighbor, end);
                heap.push(priority, nIdx);
            }
        }
    }

    return result;
}
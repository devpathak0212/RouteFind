#ifndef GRID_H
#define GRID_H

#include <vector>
#include <utility>
#include <stdexcept>

// Represents the pathfinding grid as a graph:
// each cell is a node, each walkable neighbor relationship is an edge.
// Cell value convention:
//   -1  -> wall (impassable)
//    1  -> normal cell (cost 1 to enter)
//   >1  -> weighted/difficult terrain (cost = value)
class Grid {
public:
    Grid(int rows, int cols);

    bool isValid(int row, int col) const {
        return row >= 0 && row < rows && col >= 0 && col < cols;
    }

    bool isWalkable(int row, int col) const {
        return isValid(row, col) && cells[index(row, col)] != -1;
    }

    int getCost(int row, int col) const;

    // Sets a cell's value (-1 for wall, or a positive weight).
    // Throws std::out_of_range if (row, col) is outside the grid.
    void setCell(int row, int col, int value);

    // Returns the walkable 4-directional neighbors (up/down/left/right)
    // of (row, col). Diagonal movement is intentionally excluded so that
    // Manhattan distance stays an admissible A* heuristic.
    std::vector<std::pair<int, int>> getNeighbors(int row, int col) const;

    int getRows() const { return rows; }
    int getCols() const { return cols; }

private:
    int rows, cols;
    std::vector<int> cells;

    int index(int row, int col) const { return row * cols + col; }
};

#endif
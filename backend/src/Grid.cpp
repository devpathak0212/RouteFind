#include "Grid.h"

Grid::Grid(int rows, int cols)
    : rows(rows), cols(cols), cells(rows * cols, 1) {}

int Grid::getCost(int row, int col) const {
    if (!isValid(row, col)) throw std::out_of_range("getCost: cell out of bounds");
    return cells[index(row, col)];
}

void Grid::setCell(int row, int col, int value) {
    if (!isValid(row, col)) throw std::out_of_range("setCell: cell out of bounds");
    cells[index(row, col)] = value;
}

std::vector<std::pair<int, int>> Grid::getNeighbors(int row, int col) const {
    std::vector<std::pair<int, int>> neighbors;
    const int dRow[] = {-1, 1, 0, 0};
    const int dCol[] = {0, 0, -1, 1};
    for (int i = 0; i < 4; ++i) {
        int newRow = row + dRow[i];
        int newCol = col + dCol[i];
        if (isWalkable(newRow, newCol)) neighbors.push_back({newRow, newCol});
    }
    return neighbors;
}
#include "Grid.h"
#include <iostream>
#include <cassert>

int main() {
    Grid g(5, 5);
    assert(g.isWalkable(2, 2));
    assert(g.getCost(2, 2) == 1);

    g.setCell(2, 3, -1);
    assert(!g.isWalkable(2, 3));

    g.setCell(1, 1, 5);
    assert(g.getCost(1, 1) == 5);

    auto neighbors = g.getNeighbors(2, 2);
    assert(neighbors.size() == 3);

    bool threw = false;
    try {
        g.setCell(100, 100, 1);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    auto cornerNeighbors = g.getNeighbors(0, 0);
    assert(cornerNeighbors.size() == 2);

    std::cout << "All Grid tests passed!" << std::endl;
    return 0;
}
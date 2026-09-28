#include "Grid.h"
#include "BFS.h"
#include "Dijkstra.h"
#include "AStar.h"
#include "Timer.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// Sum of the cost of entering every cell after the first.
static long pathCost(const Grid& g, const std::vector<std::pair<int, int>>& path) {
    long total = 0;
    for (size_t i = 1; i < path.size(); ++i) {
        total += g.getCost(path[i].first, path[i].second);
    }
    return total;
}

static Grid makeGrid(int size, double wallChance, double mudChance, std::mt19937& rng) {
    Grid g(size, size);
    std::uniform_real_distribution<double> roll(0.0, 1.0);
    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            bool isStart = (r == 0 && c == 0);
            bool isEnd = (r == size - 1 && c == size - 1);
            if (isStart || isEnd) continue;
            double x = roll(rng);
            if (x < wallChance) g.setCell(r, c, -1);
            else if (x < wallChance + mudChance) g.setCell(r, c, 5);
        }
    }
    return g;
}

// Run one algorithm 3 times, keep the fastest time (reduces OS noise).
// Node counts and paths are deterministic, so they come from the last run.
template <typename Solver>
static PathResult timedSolve(Solver solver, const Grid& g,
                             std::pair<int, int> s, std::pair<int, int> e) {
    PathResult best;
    double bestMs = 1e18;
    for (int i = 0; i < 3; ++i) {
        Timer t;
        t.start();
        PathResult r = solver(g, s, e);
        double ms = t.elapsedMs();
        if (ms < bestMs) bestMs = ms;
        best = r;
    }
    best.timeMs = bestMs;
    return best;
}

struct Totals {
    double nodes = 0, ms = 0, cost = 0;
};

int main() {
    struct Config { int size; double walls; double mud; };
    const std::vector<Config> configs = {
        {50, 0.00, 0.00},  {50, 0.25, 0.10},
        {100, 0.00, 0.00}, {100, 0.25, 0.10},
        {200, 0.00, 0.00}, {200, 0.25, 0.10},
    };
    const int trials = 100;

    std::mt19937 rng(12345); // fixed seed: same grids every run
    std::cout << std::fixed;

    for (const auto& cfg : configs) {
        Totals bfs, dij, ast;
        int solved = 0, attempts = 0;

        while (solved < trials && attempts < trials * 20) {
            ++attempts;
            Grid g = makeGrid(cfg.size, cfg.walls, cfg.mud, rng);
            std::pair<int, int> s = {0, 0};
            std::pair<int, int> e = {cfg.size - 1, cfg.size - 1};

            PathResult a = timedSolve(AStar::solve, g, s, e);
            if (!a.found) continue; // skip grids with no path
            PathResult d = timedSolve(Dijkstra::solve, g, s, e);
            PathResult b = timedSolve(BFS::solve, g, s, e);

            long costA = pathCost(g, a.path);
            long costD = pathCost(g, d.path);
            if (costA != costD) {
                std::cerr << "MISMATCH: A* cost " << costA << " != Dijkstra cost "
                          << costD << " (bug!)" << std::endl;
                return 1;
            }

            bfs.nodes += b.nodesExpanded; bfs.ms += b.timeMs; bfs.cost += pathCost(g, b.path);
            dij.nodes += d.nodesExpanded; dij.ms += d.timeMs; dij.cost += costD;
            ast.nodes += a.nodesExpanded; ast.ms += a.timeMs; ast.cost += costA;
            ++solved;
        }

        if (solved == 0) continue;
        const double n = solved;

        std::cout << "== " << cfg.size << "x" << cfg.size
                  << ", walls " << std::setprecision(0) << cfg.walls * 100 << "%"
                  << ", mud " << cfg.mud * 100 << "%"
                  << " (" << solved << " solvable grids) ==\n";
        std::cout << std::left << std::setw(10) << "Algorithm"
                  << std::right << std::setw(12) << "avg nodes"
                  << std::setw(11) << "avg ms" << std::setw(11) << "avg cost" << "\n";

        auto row = [&](const char* name, const Totals& t) {
            std::cout << std::left << std::setw(10) << name << std::right
                      << std::setw(12) << std::setprecision(0) << t.nodes / n
                      << std::setw(11) << std::setprecision(3) << t.ms / n
                      << std::setw(11) << std::setprecision(1) << t.cost / n << "\n";
        };
        row("BFS", bfs);
        row("Dijkstra", dij);
        row("A*", ast);

        double nodeSaving = 100.0 * (1.0 - ast.nodes / dij.nodes);
        double bfsOverhead = 100.0 * (bfs.cost / dij.cost - 1.0);
        std::cout << std::setprecision(1)
                  << "A* expanded " << nodeSaving << "% fewer nodes than Dijkstra"
                  << " (same optimal cost on every grid)\n"
                  << "BFS path cost was " << bfsOverhead << "% above optimal\n\n";
    }
    return 0;
}

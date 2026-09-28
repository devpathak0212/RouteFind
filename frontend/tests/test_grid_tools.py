import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))

from grid_model import GridModel, WALL, NORMAL
from grid_tools import random_obstacles, path_cost


def test_random_is_repeatable_with_seed():
    a, b = GridModel(20, 30), GridModel(20, 30)
    random_obstacles(a, seed=7)
    random_obstacles(b, seed=7)
    assert a.flat_grid() == b.flat_grid()


def test_random_differs_between_seeds():
    a, b = GridModel(20, 30), GridModel(20, 30)
    random_obstacles(a, seed=1)
    random_obstacles(b, seed=2)
    assert a.flat_grid() != b.flat_grid()


def test_random_keeps_start_and_end_free():
    for seed in range(20):
        g = GridModel(10, 10)
        random_obstacles(g, wall_chance=0.9, seed=seed)
        assert g.get(*g.start) == NORMAL
        assert g.get(*g.end) == NORMAL


def test_random_proportions_are_reasonable():
    g = GridModel(40, 40)
    random_obstacles(g, wall_chance=0.25, mud_chance=0.10, seed=3)
    total = 40 * 40
    walls = sum(1 for v in g.flat_grid() if v == WALL)
    mud = sum(1 for v in g.flat_grid() if v > 1)
    assert 0.20 * total < walls < 0.30 * total
    assert 0.06 * total < mud < 0.14 * total


def test_path_cost_counts_entered_cells_only():
    g = GridModel(3, 3)
    g.set_weight(0, 1, 5)
    assert path_cost(g, [(0, 0), (0, 1), (0, 2)]) == 5 + 1
    assert path_cost(g, [(0, 0)]) == 0
    assert path_cost(g, []) == 0


if __name__ == "__main__":
    tests = [v for k, v in list(globals().items()) if k.startswith("test_")]
    for t in tests:
        t()
        print("PASS", t.__name__)
    print("All grid_tools tests passed!")
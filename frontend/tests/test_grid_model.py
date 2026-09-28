import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))

from grid_model import GridModel, WALL, NORMAL


def test_new_grid_is_all_normal():
    g = GridModel(4, 6)
    assert len(g.flat_grid()) == 24
    assert all(v == NORMAL for v in g.flat_grid())
    assert g.start == (0, 0)
    assert g.end == (3, 5)


def test_set_wall_and_weight():
    g = GridModel(4, 6)
    g.set_wall(1, 2)
    g.set_weight(2, 3, 5)
    assert g.get(1, 2) == WALL
    assert g.get(2, 3) == 5
    assert g.flat_grid()[1 * 6 + 2] == WALL  # row*cols+col layout


def test_clear_cell():
    g = GridModel(4, 6)
    g.set_wall(1, 1)
    g.clear_cell(1, 1)
    assert g.get(1, 1) == NORMAL


def test_cannot_wall_over_start_or_end():
    g = GridModel(4, 6)
    g.set_wall(0, 0)
    g.set_wall(3, 5)
    assert g.get(0, 0) == NORMAL
    assert g.get(3, 5) == NORMAL


def test_out_of_bounds_is_ignored():
    g = GridModel(4, 6)
    g.set_wall(-1, 0)
    g.set_wall(4, 0)
    g.set_wall(0, 6)
    assert all(v == NORMAL for v in g.flat_grid())


def test_moving_start_and_end():
    g = GridModel(4, 6)
    g.set_wall(2, 2)
    g.set_start(2, 2)  # moving start onto a wall clears the wall
    assert g.start == (2, 2)
    assert g.get(2, 2) == NORMAL
    g.set_end(2, 2)  # cannot put end on top of start
    assert g.end == (3, 5)


def test_clear_all():
    g = GridModel(4, 6)
    g.set_wall(1, 1)
    g.set_weight(2, 2, 5)
    g.clear_all()
    assert all(v == NORMAL for v in g.flat_grid())


if __name__ == "__main__":
    tests = [v for k, v in list(globals().items()) if k.startswith("test_")]
    for t in tests:
        t()
        print("PASS", t.__name__)
    print("All grid_model tests passed!")
import os
import sys

# Run pygame without opening a real window
os.environ["SDL_VIDEODRIVER"] = "dummy"
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))

import pygame
from grid_model import GridModel
from grid_view import (GridView, WALL_COLOR, MUD_COLOR, START_COLOR,
                       END_COLOR, PATH_COLOR, NORMAL_COLOR)


def center_color(surface, view, row, col):
    rect = view.cell_rect(row, col)
    return tuple(surface.get_at(rect.center))[:3]


def test_pixel_to_cell_inside():
    view = GridView(GridModel(5, 8), cell_size=20, origin=(10, 10))
    assert view.pixel_to_cell(10, 10) == (0, 0)      # top-left corner
    assert view.pixel_to_cell(29, 29) == (0, 0)      # still inside first cell
    assert view.pixel_to_cell(30, 10) == (0, 1)      # next column
    assert view.pixel_to_cell(10, 30) == (1, 0)      # next row
    assert view.pixel_to_cell(169, 109) == (4, 7)    # bottom-right cell


def test_pixel_to_cell_outside():
    view = GridView(GridModel(5, 8), cell_size=20, origin=(10, 10))
    assert view.pixel_to_cell(9, 50) is None         # left of grid
    assert view.pixel_to_cell(50, 9) is None         # above grid
    assert view.pixel_to_cell(170, 50) is None       # right of grid
    assert view.pixel_to_cell(50, 110) is None       # below grid


def test_draw_uses_right_colors():
    pygame.init()
    model = GridModel(5, 8)
    model.set_wall(1, 1)
    model.set_weight(2, 2, 5)
    view = GridView(model, cell_size=20, origin=(0, 0))
    surface = pygame.Surface(view.pixel_size())
    view.draw(surface, path=[(0, 0), (0, 1), (0, 2)])

    assert center_color(surface, view, 0, 0) == START_COLOR
    assert center_color(surface, view, 4, 7) == END_COLOR
    assert center_color(surface, view, 1, 1) == WALL_COLOR
    assert center_color(surface, view, 2, 2) == MUD_COLOR
    assert center_color(surface, view, 0, 1) == PATH_COLOR
    assert center_color(surface, view, 3, 3) == NORMAL_COLOR


def test_path_does_not_hide_start_and_end():
    pygame.init()
    model = GridModel(3, 3)
    view = GridView(model, cell_size=20)
    surface = pygame.Surface(view.pixel_size())
    view.draw(surface, path=[(0, 0), (1, 1), (2, 2)])
    assert center_color(surface, view, 0, 0) == START_COLOR
    assert center_color(surface, view, 2, 2) == END_COLOR


if __name__ == "__main__":
    tests = [v for k, v in list(globals().items()) if k.startswith("test_")]
    for t in tests:
        t()
        print("PASS", t.__name__)
    print("All grid_view tests passed!")
import pygame

# Colors
BG_COLOR = (30, 30, 36)
NORMAL_COLOR = (235, 235, 240)
WALL_COLOR = (40, 44, 60)
MUD_COLOR = (150, 105, 60)
START_COLOR = (60, 190, 90)
END_COLOR = (220, 70, 70)
PATH_COLOR = (80, 150, 255)


class GridView:
    """Draws a GridModel and converts mouse pixels to grid cells."""

    def __init__(self, model, cell_size=24, origin=(0, 0)):
        self.model = model
        self.cell_size = cell_size
        self.origin = origin  # top-left pixel of the grid

    def pixel_size(self):
        return (self.model.cols * self.cell_size, self.model.rows * self.cell_size)

    def pixel_to_cell(self, x, y):
        """Return (row, col) for a pixel position, or None if outside the grid."""
        col = (x - self.origin[0]) // self.cell_size
        row = (y - self.origin[1]) // self.cell_size
        if self.model.in_bounds(row, col) and x >= self.origin[0] and y >= self.origin[1]:
            return (row, col)
        return None

    def cell_rect(self, row, col):
        return pygame.Rect(
            self.origin[0] + col * self.cell_size,
            self.origin[1] + row * self.cell_size,
            self.cell_size,
            self.cell_size,
        )

    def _color_for(self, row, col):
        value = self.model.get(row, col)
        if value == -1:
            return WALL_COLOR
        if value > 1:
            return MUD_COLOR
        return NORMAL_COLOR

    def draw(self, surface, path=None):
        path_cells = set(path) if path else set()
        gap = 1  # thin gap between cells makes grid lines

        for row in range(self.model.rows):
            for col in range(self.model.cols):
                if (row, col) == self.model.start:
                    color = START_COLOR
                elif (row, col) == self.model.end:
                    color = END_COLOR
                elif (row, col) in path_cells:
                    color = PATH_COLOR
                else:
                    color = self._color_for(row, col)

                rect = self.cell_rect(row, col).inflate(-gap * 2, -gap * 2)
                pygame.draw.rect(surface, color, rect)
WALL = -1
NORMAL = 1


class GridModel:
    """Holds the grid data and the start/end points. No drawing code here."""

    def __init__(self, rows, cols):
        self.rows = rows
        self.cols = cols
        self.cells = [NORMAL] * (rows * cols)  # flat list, same layout as the C++ Grid
        self.start = (0, 0)
        self.end = (rows - 1, cols - 1)

    def in_bounds(self, row, col):
        return 0 <= row < self.rows and 0 <= col < self.cols

    def get(self, row, col):
        return self.cells[row * self.cols + col]

    def _is_endpoint(self, row, col):
        return (row, col) == self.start or (row, col) == self.end

    def set_wall(self, row, col):
        if self.in_bounds(row, col) and not self._is_endpoint(row, col):
            self.cells[row * self.cols + col] = WALL

    def set_weight(self, row, col, weight):
        if self.in_bounds(row, col) and not self._is_endpoint(row, col):
            self.cells[row * self.cols + col] = weight

    def clear_cell(self, row, col):
        if self.in_bounds(row, col):
            self.cells[row * self.cols + col] = NORMAL

    def set_start(self, row, col):
        # start must be inside the grid, not on the end, and not a wall
        if self.in_bounds(row, col) and (row, col) != self.end:
            self.clear_cell(row, col)
            self.start = (row, col)

    def set_end(self, row, col):
        if self.in_bounds(row, col) and (row, col) != self.start:
            self.clear_cell(row, col)
            self.end = (row, col)

    def clear_all(self):
        self.cells = [NORMAL] * (self.rows * self.cols)

    def flat_grid(self):
        """The list that api_client.solve() expects."""
        return list(self.cells)
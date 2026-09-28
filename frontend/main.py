import sys

import pygame

import api_client
from api_client import BackendError
from grid_model import GridModel
from grid_view import GridView, BG_COLOR
from grid_tools import random_obstacles, path_cost

ROWS, COLS, CELL = 22, 36, 24
BAR_HEIGHT = 150
MUD_WEIGHT = 5

ALGORITHM_ORDER = ["bfs", "dijkstra", "astar"]
ALGORITHM_KEYS = {pygame.K_1: "bfs", pygame.K_2: "dijkstra", pygame.K_3: "astar"}
ALGORITHM_LABELS = {"bfs": "BFS", "dijkstra": "Dijkstra", "astar": "A*"}
BRUSH_KEYS = {pygame.K_w: "wall", pygame.K_m: "mud", pygame.K_s: "start", pygame.K_e: "end"}

TEXT_COLOR = (225, 225, 232)
DIM_COLOR = (150, 150, 165)
ERROR_COLOR = (255, 120, 120)
BAR_COLOR = (22, 22, 28)


class App:
    def __init__(self):
        pygame.font.init()
        self.model = GridModel(ROWS, COLS)
        self.view = GridView(self.model, cell_size=CELL, origin=(0, 0))
        self.algorithm = "astar"
        self.brush = "wall"
        self.results = {}   # algorithm name -> result dict for the CURRENT grid
        self.reveal = 0     # how many path cells are shown (for the animation)
        self.message = ""
        self.font = pygame.font.SysFont("consolas,couriernew,monospace", 16)

    def window_size(self):
        width, height = self.view.pixel_size()
        return (width, height + BAR_HEIGHT)

    # ---------- state changes ----------
    def _snapshot(self):
        return (tuple(self.model.cells), self.model.start, self.model.end)

    def _grid_changed(self):
        self.results = {}
        self.reveal = 0
        self.message = ""

    def paint(self, pos, erase=False):
        cell = self.view.pixel_to_cell(*pos)
        if cell is None:
            return
        row, col = cell
        before = self._snapshot()
        if erase:
            self.model.clear_cell(row, col)
        elif self.brush == "wall":
            self.model.set_wall(row, col)
        elif self.brush == "mud":
            self.model.set_weight(row, col, MUD_WEIGHT)
        elif self.brush == "start":
            self.model.set_start(row, col)
        elif self.brush == "end":
            self.model.set_end(row, col)
        if self._snapshot() != before:
            self._grid_changed()

    def solve(self):
        try:
            result = api_client.solve(
                self.model.flat_grid(), self.model.rows, self.model.cols,
                self.model.start, self.model.end, self.algorithm,
            )
        except BackendError as error:
            self.message = str(error)
            return
        self.message = ""
        if result["found"]:
            result["cost"] = path_cost(self.model, result["path"])
        self.results[self.algorithm] = result
        self.reveal = 0  # restart the path animation

    def select_algorithm(self, name):
        self.algorithm = name
        result = self.results.get(name)
        self.reveal = len(result["path"]) if result and result["found"] else 0

    # ---------- events / per-frame ----------
    def handle_event(self, event):
        """Returns False when the app should quit."""
        if event.type == pygame.QUIT:
            return False
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                return False
            if event.key in ALGORITHM_KEYS:
                self.select_algorithm(ALGORITHM_KEYS[event.key])
            elif event.key in BRUSH_KEYS:
                self.brush = BRUSH_KEYS[event.key]
            elif event.key in (pygame.K_SPACE, pygame.K_RETURN):
                self.solve()
            elif event.key == pygame.K_c:
                self.model.clear_all()
                self._grid_changed()
            elif event.key == pygame.K_g:
                random_obstacles(self.model)
                self._grid_changed()
        elif event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:
                self.paint(event.pos)
            elif event.button == 3:
                self.paint(event.pos, erase=True)
        elif event.type == pygame.MOUSEMOTION:
            if event.buttons[0]:
                self.paint(event.pos)
            elif event.buttons[2]:
                self.paint(event.pos, erase=True)
        return True

    def update(self):
        result = self.results.get(self.algorithm)
        if result and result["found"] and self.reveal < len(result["path"]):
            self.reveal = min(len(result["path"]), self.reveal + 3)

    # ---------- drawing ----------
    def _result_line(self, name):
        marker = ">" if name == self.algorithm else " "
        label = ALGORITHM_LABELS[name]
        result = self.results.get(name)
        if result is None:
            return f"{marker} {label:<9} -"
        if not result["found"]:
            return f"{marker} {label:<9} no path | nodes {result['nodesExpanded']}"
        return (f"{marker} {label:<9} cost {result['cost']:>4} | cells {len(result['path']):>3}"
                f" | nodes {result['nodesExpanded']:>5} | {result['timeMs']:.3f} ms")

    def draw(self, screen):
        screen.fill(BG_COLOR)
        result = self.results.get(self.algorithm)
        shown = result["path"][:self.reveal] if result and result["found"] else None
        self.view.draw(screen, path=shown)

        grid_w, grid_h = self.view.pixel_size()
        pygame.draw.rect(screen, BAR_COLOR, pygame.Rect(0, grid_h, grid_w, BAR_HEIGHT))

        lines = [
            (f"Algorithm: {ALGORITHM_LABELS[self.algorithm]}   Brush: {self.brush}", TEXT_COLOR),
            ("1 BFS   2 Dijkstra   3 A*   |   W wall   M mud   S start   E end", DIM_COLOR),
            ("Left-drag paint   Right-drag erase   Space solve   G random   C clear", DIM_COLOR),
        ]
        for name in ALGORITHM_ORDER:
            lines.append((self._result_line(name), TEXT_COLOR))
        if self.message:
            lines.append((self.message, ERROR_COLOR))

        y = grid_h + 8
        for text, color in lines:
            screen.blit(self.font.render(text, True, color), (10, y))
            y += 20

    def run(self):
        pygame.init()
        screen = pygame.display.set_mode(self.window_size())
        pygame.display.set_caption("RouteFind")
        clock = pygame.time.Clock()
        running = True
        while running:
            for event in pygame.event.get():
                if not self.handle_event(event):
                    running = False
            self.update()
            self.draw(screen)
            pygame.display.flip()
            clock.tick(60)
        pygame.quit()


if __name__ == "__main__":
    App().run()
    sys.exit(0)
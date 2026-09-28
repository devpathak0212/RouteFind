import random


def random_obstacles(model, wall_chance=0.25, mud_chance=0.10, mud_weight=5, seed=None):
    """Clear the grid, then scatter random walls and mud. Start and end stay free."""
    rng = random.Random(seed)
    model.clear_all()
    for row in range(model.rows):
        for col in range(model.cols):
            if (row, col) == model.start or (row, col) == model.end:
                continue
            roll = rng.random()
            if roll < wall_chance:
                model.set_wall(row, col)
            elif roll < wall_chance + mud_chance:
                model.set_weight(row, col, mud_weight)


def path_cost(model, path):
    """Total cost of a path: the cost of entering every cell after the first."""
    return sum(model.get(row, col) for row, col in path[1:])
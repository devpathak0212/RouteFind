import requests

BASE_URL = "http://localhost:8081"


class BackendError(Exception):
    """Raised when the C++ backend can't be reached or rejects a request."""


def solve(grid, rows, cols, start, end, algorithm, timeout=10):
    """Send a grid to the C++ backend and return its result as a dict.

    grid      -- flat list of ints, length rows*cols (-1 wall, 1 normal, >1 weighted)
    start/end -- (row, col) tuples
    algorithm -- "bfs", "dijkstra" or "astar"

    Returns {"found", "path", "nodesExpanded", "timeMs"} with path as a list of (row, col) tuples.
    """
    payload = {
        "rows": rows,
        "cols": cols,
        "grid": list(grid),
        "start": list(start),
        "end": list(end),
        "algorithm": algorithm,
    }

    try:
        response = requests.post(BASE_URL + "/solve", json=payload, timeout=timeout)
    except requests.exceptions.ConnectionError:
        raise BackendError("Cannot reach the backend. Is routefind_server.exe running?")
    except requests.exceptions.Timeout:
        raise BackendError("The backend took too long to respond.")

    try:
        data = response.json()
    except ValueError:
        raise BackendError("Backend sent back something that is not JSON.")

    if response.status_code != 200:
        raise BackendError(data.get("error", "Backend returned an error."))

    data["path"] = [tuple(cell) for cell in data["path"]]
    return data
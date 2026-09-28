import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))

import api_client
from api_client import BackendError

# 3x5 grid with a block of mud in the middle
GRID = [1, 5, 5, 1, 1,
        1, 5, 5, 1, 1,
        1, 1, 1, 1, 1]


def test_astar_finds_path_around_mud():
    r = api_client.solve(GRID, 3, 5, (0, 0), (2, 4), "astar")
    assert r["found"] is True
    assert r["path"][0] == (0, 0)
    assert r["path"][-1] == (2, 4)
    assert (0, 1) not in r["path"]  # avoided the mud
    assert r["nodesExpanded"] > 0


def test_all_three_algorithms_respond():
    for name in ("bfs", "dijkstra", "astar"):
        r = api_client.solve(GRID, 3, 5, (0, 0), (2, 4), name)
        assert r["found"] is True, name


def test_unreachable_target():
    walled = [1, 1, 1,
              -1, -1, -1,
              1, 1, 1]
    r = api_client.solve(walled, 3, 3, (0, 0), (2, 0), "astar")
    assert r["found"] is False
    assert r["path"] == []


def test_bad_algorithm_raises():
    try:
        api_client.solve(GRID, 3, 5, (0, 0), (2, 4), "bogus")
        assert False, "should have raised BackendError"
    except BackendError:
        pass


def test_server_down_raises():
    original = api_client.BASE_URL
    api_client.BASE_URL = "http://localhost:1"  # nothing listens here
    try:
        api_client.solve(GRID, 3, 5, (0, 0), (2, 4), "astar")
        assert False, "should have raised BackendError"
    except BackendError:
        pass
    finally:
        api_client.BASE_URL = original


if __name__ == "__main__":
    tests = [v for k, v in list(globals().items()) if k.startswith("test_")]
    for t in tests:
        t()
        print("PASS", t.__name__)
    print("All api_client tests passed!")
#include "httplib.h"
#include "json.hpp"
#include "Grid.h"
#include "BFS.h"
#include "Dijkstra.h"
#include "AStar.h"
#include "Timer.h"
#include <iostream>

using json = nlohmann::json;

static json pathResultToJson(const PathResult& result) {
    json response;
    response["found"] = result.found;
    response["nodesExpanded"] = result.nodesExpanded;
    response["timeMs"] = result.timeMs;

    json pathArray = json::array();
    for (const auto& cell : result.path) {
        pathArray.push_back({cell.first, cell.second});
    }
    response["path"] = pathArray;

    return response;
}

int main() {
    httplib::Server server;

    server.Post("/solve", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Content-Type", "application/json");

        json body;
        try {
            body = json::parse(req.body);
        } catch (const json::parse_error&) {
            res.status = 400;
            res.set_content(R"({"error":"invalid JSON"})", "application/json");
            return;
        }

        if (!body.contains("rows") || !body.contains("cols") ||
            !body.contains("grid") || !body.contains("start") ||
            !body.contains("end") || !body.contains("algorithm")) {
            res.status = 400;
            res.set_content(R"({"error":"missing required field"})", "application/json");
            return;
        }

        try {
            int rows = body["rows"].get<int>();
            int cols = body["cols"].get<int>();
            std::vector<int> cellValues = body["grid"].get<std::vector<int>>();

            if (static_cast<int>(cellValues.size()) != rows * cols) {
                res.status = 400;
                res.set_content(R"({"error":"grid size does not match rows*cols"})", "application/json");
                return;
            }

            Grid grid(rows, cols);
            for (int r = 0; r < rows; ++r) {
                for (int c = 0; c < cols; ++c) {
                    grid.setCell(r, c, cellValues[r * cols + c]);
                }
            }

            std::pair<int, int> start = {body["start"][0].get<int>(), body["start"][1].get<int>()};
            std::pair<int, int> end = {body["end"][0].get<int>(), body["end"][1].get<int>()};
            std::string algorithm = body["algorithm"].get<std::string>();

            Timer timer;
            timer.start();

            PathResult result;
            if (algorithm == "bfs") {
                result = BFS::solve(grid, start, end);
            } else if (algorithm == "dijkstra") {
                result = Dijkstra::solve(grid, start, end);
            } else if (algorithm == "astar") {
                result = AStar::solve(grid, start, end);
            } else {
                res.status = 400;
                res.set_content(R"({"error":"unknown algorithm, expected bfs/dijkstra/astar"})", "application/json");
                return;
            }

            result.timeMs = timer.elapsedMs();

            res.set_content(pathResultToJson(result).dump(), "application/json");

        } catch (const std::exception& e) {
            res.status = 400;
            json err;
            err["error"] = std::string("bad request: ") + e.what();
            res.set_content(err.dump(), "application/json");
        }
    });

    std::cout << "RouteFind backend listening on http://localhost:8081" << std::endl;
    server.listen("0.0.0.0", 8081);

    return 0;
}

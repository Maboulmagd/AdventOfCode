#include "height_map.h"

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <unordered_set>

using namespace AOC2022Day12HeightMap;

std::size_t ShortestStepsToTarget(const std::vector<std::string>& height_map, std::vector<HeightMapPosition>& starts, HeightMapPosition end, bool multiple_sources = false) {

    std::vector<std::vector<int32_t>> visited(height_map.size(), std::vector<int32_t>(height_map[0].size(), INT32_MAX));

    const auto within_bounds = [&height_map](const HeightMapPosition& target_position) {
        // Check bounds first
        if (target_position.x_ < 0 || target_position.x_ == height_map.size()) return false;
        if (target_position.y_ < 0 || target_position.y_ == height_map[target_position.x_].size()) return false;
        return true;
    };

    const auto elevation_ok = []
            (const HeightMapPosition& curr_position, const HeightMapPosition& target_position) {
        if (curr_position.val_ >= target_position.val_) return true;// If our current position's value is >= target position's value, we are ok
        return curr_position.val_ + 1 == target_position.val_;// Only 1 more than current is acceptable
    };

    const auto valid_position = [&height_map, &visited, &elevation_ok]
            (const HeightMapPosition& curr_position, const HeightMapPosition& target_position) {
        if (target_position.num_steps_ >= visited[target_position.x_][target_position.y_]) return false;// Already have a shorter path
        if (curr_position.val_ == 'S') {
            return elevation_ok({'a', 0, 0}, target_position);// Can we hop from 'a' to target?
        }
        if (target_position.val_ == 'E') {
            // Can we hop from curr_position to 'E', which has elevation 'z'?
            return elevation_ok({height_map[curr_position.x_][curr_position.y_], 0, 0}, {'z', 0, 0});
        }
        if (!elevation_ok(curr_position, target_position)) return false;// Standard case
        return true;
    };

    // BFS guarantees the shortest number of steps, but we can revisit positions/cells from other routes that result in shorter distances
    std::queue<HeightMapPosition> q;
    if (multiple_sources) {
        for (const auto& start : starts) {
            q.push(start);
            visited[start.x_][start.y_] = 0;
        }
    }
    else {
        for (const auto& start : starts) {
            if (start.val_ == 'S') {
                q.push(start);
                visited[start.x_][start.y_] = 0;
            }
        }
    }

    while (!q.empty()) {
        const HeightMapPosition p = q.front();
        q.pop();

        if (p.val_ == 'E') {
            continue;
        }

        // Try to go in 4 directions
        for (auto dir : {p.Up(), p.Down(), p.Left(), p.Right()}) {
            // Set value of the position, assuming it is within bounds
            if (!within_bounds(dir)) continue;
            dir.val_ = height_map[dir.x_][dir.y_];

            // Set the number of steps
            dir.num_steps_ = p.num_steps_ + 1;

            // Check if we can go in that direction
            if (!valid_position(p, dir)) continue;

            q.push(dir);
            visited[dir.x_][dir.y_] = dir.num_steps_;
        }
    }

    return visited[end.x_][end.y_];
}

int ParseAndRun(const std::string& path) {
    std::fstream file_stream(path);

    if (!file_stream.is_open()) {
        std::cerr << "Failed to open: " << std::quoted(path) << std::endl;
        return 1;
    }

    std::vector<std::string> height_map;
    std::string line;
    std::vector<HeightMapPosition> starting_positions;
    HeightMapPosition ending_pos{};
    int32_t row = 0;

    while (std::getline(file_stream, line)) {
        for (int32_t col = 0; col < line.size(); ++col) {
            if (line[col] == 'S') {
                starting_positions.emplace_back(HeightMapPosition{'S', row, col, 0});
            }
            else if (line[col] == 'a') {
                starting_positions.emplace_back(HeightMapPosition{'a', row, col, 0});
            }
            else if (line[col] == 'E') {
                ending_pos = {line[col], row, col, 0};
            }
        }
        ++row;

        height_map.push_back(line);
    }

    std::cout << ShortestStepsToTarget(height_map, starting_positions, ending_pos) << std::endl;
    std::cout << ShortestStepsToTarget(height_map, starting_positions, ending_pos, true) << std::endl;

    return 0;
}

int Test() {
    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return Test();
    }

    return ParseAndRun(argv[1]);
}
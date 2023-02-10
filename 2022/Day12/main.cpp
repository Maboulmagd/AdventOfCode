#include "height_map.h"

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <unordered_set>

using namespace AOC2022Day12HeightMap;

std::size_t ShortestStepsToTarget(const std::vector<std::string>& height_map, HeightMapPosition start, HeightMapPosition end) {

    std::unordered_set<HeightMapPosition> visited;

    const auto within_bounds = [&height_map](const HeightMapPosition& target_position) {
        // Check bounds first
        if (target_position.x_ < 0 || target_position.x_ == height_map.size()) return false;
        if (target_position.y_ < 0 || target_position.y_ == height_map[target_position.x_].size()) return false;
        return true;
    };

    const auto elevation_ok = []
            (const HeightMapPosition& curr_position, const HeightMapPosition& target_position) {
        if (curr_position.val_ == 'S') return true;
        if (target_position.val_ == 'E') return true;
        if (curr_position.val_ >= target_position.val_) return true;
        return curr_position.val_ + 1 == target_position.val_;// Only 1 more than current is acceptable
    };

    const auto valid_position = [&height_map, &visited, &elevation_ok]
            (const HeightMapPosition& curr_position, const HeightMapPosition& target_position) {
        if (visited.contains(target_position)) return false;
        if (!elevation_ok(curr_position, target_position)) return false;
        return true;
    };

    // BFS guarantees the shortest number of steps
    std::queue<HeightMapPosition> q;
    q.push(start);
    visited.insert(start);

    while (!q.empty()) {
        const HeightMapPosition p = q.front();
        q.pop();

        if (p.val_ == 'E') {
            end.num_steps_ = p.num_steps_;
            break;
        }

        // Try to go in 4 directions
        for (auto dir : {p.Up(), p.Down(), p.Left(), p.Right()}) {
            // Set value of the position, assuming it is within bounds
            if (!within_bounds(dir)) continue;
            dir.val_ = height_map[dir.x_][dir.y_];

            // Check if we can go in that direction (has not been previously visited, and valid)
            if (!valid_position(p, dir)) continue;

            // Set the number of steps
            dir.num_steps_ = p.num_steps_ + 1;

            q.push(dir);
            visited.insert(dir);
        }
    }

    while (!q.empty()) {
        q.pop();
    }

    return end.num_steps_;
}

int ParseAndRun(const std::string& path) {
    std::fstream file_stream(path);

    if (!file_stream.is_open()) {
        std::cerr << "Failed to open: " << std::quoted(path) << std::endl;
        return 1;
    }

    std::vector<std::string> height_map;
    std::string line;
    HeightMapPosition starting_pos{};
    HeightMapPosition ending_pos{};
    int32_t row = 0;
    while (std::getline(file_stream, line)) {
        for (int32_t col = 0; col < line.size(); ++col) {
            if (line[col] == 'S') {
                starting_pos = {line[col], row, col, 0};
            }
            else if (line[col] == 'E') {
                ending_pos = {line[col], row, col, 0};
            }
        }
        ++row;

        height_map.push_back(line);
    }

    std::cout << ShortestStepsToTarget(height_map, starting_pos, ending_pos) << std::endl;
    //std::cout << ShortestStepsToTarget(height_map, starting_pos, ending_pos) << std::endl;

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
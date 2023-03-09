#include "cave_map.h"

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <ranges>

using namespace AOC2022Day14CaveMap;

uint32_t CountUnitsOfSand(CaveMap& cave_map, bool expanded_floor = false) {
    uint32_t units_of_sand = 0;

    const auto trickle_sand = [&cave_map, &expanded_floor](int32_t& row, int32_t& col) -> bool {
        // Is sand within bounds
        if (!expanded_floor && ((row > cave_map.max_row_) || (col < cave_map.min_col_ || col > cave_map.max_col_))) { return false; }

        if (expanded_floor && ((row < 0 || row >= cave_map.map_.size()) || (col < 0 || col >= cave_map.map_[0].size()))) {
            throw std::runtime_error("Out of bounds, need bigger cave map.");
        }

        if (expanded_floor && row + 1 == cave_map.max_row_ + 2) {
            cave_map.map_[row][col] = 'O';
            return false;
        }

        // Can sand go down
        if (cave_map.map_[row+1][col] == '.') {
            ++row;
        }
        // Can sand go down-left
        else if (cave_map.map_[row+1][col-1] == '.') {
            ++row;
            --col;
        }
        // Can sand go down-right
        else if (cave_map.map_[row+1][col+1] == '.') {
            ++row;
            ++col;
        }
        // We're stuck
        else {
            cave_map.map_[row][col] = 'O';
            return false;
        }

        return true;
    };

    while (true) {
        int32_t row = 0;
        int32_t col = 500;

        while (trickle_sand(row, col));
        if (cave_map.map_[row][col] == 'O') {
            ++units_of_sand;
            if (expanded_floor && row == 0 && col == 500) {
                break;
            }
        }
        else {
            break;
        }
    }

    return units_of_sand;
}

int ParseAndRun(const std::string& path) {
    std::fstream file_stream(path);

    if (!file_stream.is_open()) {
        std::cerr << "Failed to open: " << std::quoted(path) << std::endl;
        return 1;
    }

    CaveMap cave_map;
    std::string line;

    while (std::getline(file_stream, line)) {
        cave_map.AddScan(line);
    }

    std::cout << CountUnitsOfSand(cave_map) << std::endl;

    cave_map.Reset();// Reset sand poured into cave from part 1
    std::cout << CountUnitsOfSand(cave_map, true) << std::endl;

    return 0;
}

int Test() {
    std::stringstream ss(R"(498,4 -> 498,6 -> 496,6
    503,4 -> 502,4 -> 502,9 -> 494,9)"
    );

    CaveMap cave_map;
    std::string line;

    while (std::getline(ss, line)) {
        cave_map.AddScan(line);
    }

    assert(CountUnitsOfSand(cave_map) == 24);

    cave_map.Reset();// Reset sand poured into cave from part 1
    assert(CountUnitsOfSand(cave_map, true) == 93);

    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return Test();
    }

    return ParseAndRun(argv[1]);
}
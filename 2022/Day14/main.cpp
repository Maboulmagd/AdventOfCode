#include "cave_map.h"

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cassert>

using namespace AOC2022Day14CaveMap;

uint32_t CountUnitsOfSand(CaveMap& cave_map) {
    uint32_t units_of_sand = 0;

    const auto trickle_sand = [&cave_map](int32_t& row, int32_t& col) -> bool {
        // Is sand within bounds
        if (!cave_map.WithinCaveBounds(row, col)) {
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
    //std::cout << CountUnitsOfSand(cave_map) << std::endl;

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

    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return Test();
    }

    return ParseAndRun(argv[1]);
}
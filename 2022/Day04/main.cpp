#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <cassert>
#include <algorithm>
#include <ranges>

#include "interval.h"

using namespace AOC2022Day04Interval;

constexpr int32_t NumberOfPairsFullyContained(const std::vector<std::pair<Interval,Interval>>& interval_pairs) {
    return std::ranges::count_if(interval_pairs, [](const std::pair<Interval,Interval>& interval_pair){
        return interval_pair.first.Subsumes(interval_pair.second) || interval_pair.second.Subsumes(interval_pair.first);
    });
}

constexpr int32_t NumberOfPairsOverlapping(const std::vector<std::pair<Interval,Interval>>& interval_pairs) {
    return std::ranges::count_if(interval_pairs, [](const std::pair<Interval,Interval>& interval_pair) {
       return interval_pair.first.Overlaps(interval_pair.second) || interval_pair.second.Overlaps(interval_pair.first);
    });
}

int Test() {
    std::vector<std::pair<Interval,Interval>> test_data{
            {{2,4},{6,8}},
            {{2,3},{4,5}},
            {{5,7},{7,9}},
            {{2,8},{3,7}},
            {{6,6},{4,6}},
            {{2,6},{4,8}}
    };

    assert(NumberOfPairsFullyContained(test_data) == 2);
    assert(NumberOfPairsOverlapping(test_data) == 4);
    return 0;
}

int ParseAndRun(const std::string& path) {

    std::fstream input_file(path);

    if (!input_file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }

    std::vector<std::pair<Interval,Interval>> input;

    char delimiter;
    Interval first;
    Interval second;

    while (input_file >> first >> delimiter >> second) {
        input.emplace_back(first, second);
    }

    std::cout << NumberOfPairsFullyContained(input) << std::endl;
    std::cout << NumberOfPairsOverlapping(input) << std::endl;

    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return Test();
    }

    return ParseAndRun(argv[1]);
}
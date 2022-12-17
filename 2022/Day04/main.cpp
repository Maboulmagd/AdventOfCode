#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ranges>
#include <numeric>
#include <cassert>
#include <utility>
#include <iomanip>

struct Interval final {
    int32_t min;
    int32_t max;

    bool Subsumes(const Interval& other) const {
        return (min <= other.min) && (max >= other.max);
    }

    bool Overlaps(const Interval& other) const {
        return Subsumes(other) || ((max >= other.min) && (max <= other.max)) || ((min <= other.max) && (min >= other.min));
    }

    friend std::istream& operator>>(std::istream& file_stream, Interval& interval) {
        char delimiter;
        file_stream >> interval.min >> delimiter >> interval.max;
        return file_stream;
    }
};

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

int main() {

    std::fstream input_file("input.txt");

    if (!input_file.is_open()) {
        //std::cerr << "Failed to open " << std::quoted(input_file) << "\n";
        return 1;
    }

    std::vector<std::pair<Interval,Interval>> input;

    char delimiter;
    Interval first;
    Interval second;

    while (input_file >> first >> delimiter >> second) {
        input.push_back(std::make_pair(first, second));
    }

    std::cout << NumberOfPairsFullyContained(input) << std::endl;
    std::cout << NumberOfPairsOverlapping(input) << std::endl;

    return 0;
}
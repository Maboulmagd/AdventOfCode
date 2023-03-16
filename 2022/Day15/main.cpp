#include "sensors_and_beacons_map.h"

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <optional>
#include <ranges>
#include <algorithm>
#include <numeric>

using namespace AOC2022Day15SensorsAndBeaconsMap;

std::vector<Interval> CoverageAtRow(const std::vector<std::pair<Position, Position>>& sensors, const int64_t row) {
    // Lambda to compute coverage, each sensor should apply this function
    const auto compute_interval = [row](const std::pair<Position, Position>& sensor_beacon) -> std::optional<Interval> {
        const Position sensor = sensor_beacon.first;
        const Position beacon = sensor_beacon.second;

        const int64_t sensor_beacon_distance = sensor.Distance(beacon);
        if (std::abs(sensor.y - row) > sensor_beacon_distance) {// No coverage for that row from THIS sensor/beacon
            return std::nullopt;
        }

        const int64_t y_diff = std::abs(sensor.y - row);
        const int64_t range_rem = sensor_beacon_distance - y_diff;// Remainder to play with from distance

        return Interval{sensor.x - range_rem, sensor.x + range_rem}; // Row 11 should be {3, 13}, so abs(7 - 11) = 4, total manhattan dist (9) - 4) = 5, so 5 - sensor.x for start, and sensor.x + 5 for end
    };

    const auto has_value = [](const auto &opt) { return opt.has_value(); };
    const auto get_value = [](const auto &opt) { return *opt; };

    std::vector<Interval> intervals;
    std::ranges::transform(sensors | std::views::transform(compute_interval) | std::views::filter(has_value), std::back_inserter(intervals), get_value);

    if (intervals.empty()) {
        return {};
    }

    // Sort intervals and merge/join them
    std::ranges::sort(intervals);
    std::vector<Interval> merged_intervals;

    auto it = intervals.cbegin();
    Interval prev = *it;
    ++it;

    while (it != intervals.cend()) {
        Interval curr = *it;
        if (prev.Overlaps(curr)) {
            prev = prev.Merge(curr);
        }
        else {
            merged_intervals.push_back(prev);
            prev = *it;
        }

        ++it;
    }
    merged_intervals.push_back(prev);

    return merged_intervals;
}

int64_t PositionsThatCannotContainBeacon(const std::vector<std::pair<Position, Position>>& sensors, const int64_t row) {
    const std::vector<Interval> res = CoverageAtRow(sensors, row);

    return std::transform_reduce(res.begin(), res.end(), int64_t(0), std::plus{}, [](const Interval& it) {
        return it.Length();
    });
}

int ParseAndRun(const std::string& path) {
    std::fstream file_stream(path);

    if (!file_stream.is_open()) {
        std::cerr << "Failed to open: " << std::quoted(path) << std::endl;
        return 1;
    }

    std::vector<std::pair<Position, Position>> sensors;

    std::pair<Position, Position> sensor_and_closest_beacon;
    while (file_stream >> sensor_and_closest_beacon) {
        sensors.emplace_back(std::move(sensor_and_closest_beacon));
    }

    //std::cout << PositionsThatCannotContainBeacon(sensors, 10) << std::endl;

    std::cout << PositionsThatCannotContainBeacon(sensors, 2000000) << std::endl;
    //std::cout << PositionsThatCannotContainBeacon(sensors, 2000000) << std::endl;

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
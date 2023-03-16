#include "sensors_and_beacons_map.h"

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <optional>
#include <ranges>
#include <algorithm>
#include <numeric>
#include <cassert>

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

    std::cout << PositionsThatCannotContainBeacon(sensors, 2000000) << std::endl;
    //std::cout << PositionsThatCannotContainBeacon(sensors, 2000000) << std::endl;

    return 0;
}

int Test() {
    std::stringstream ss(R"(Sensor at x=2, y=18: closest beacon is at x=-2, y=15
Sensor at x=9, y=16: closest beacon is at x=10, y=16
Sensor at x=13, y=2: closest beacon is at x=15, y=3
Sensor at x=12, y=14: closest beacon is at x=10, y=16
Sensor at x=10, y=20: closest beacon is at x=10, y=16
Sensor at x=14, y=17: closest beacon is at x=10, y=16
Sensor at x=8, y=7: closest beacon is at x=2, y=10
Sensor at x=2, y=0: closest beacon is at x=2, y=10
Sensor at x=0, y=11: closest beacon is at x=2, y=10
Sensor at x=20, y=14: closest beacon is at x=25, y=17
Sensor at x=17, y=20: closest beacon is at x=21, y=22
Sensor at x=16, y=7: closest beacon is at x=15, y=3
Sensor at x=14, y=3: closest beacon is at x=15, y=3
Sensor at x=20, y=1: closest beacon is at x=15, y=3)"
    );

    std::vector<std::pair<Position, Position>> sensors;

    std::pair<Position, Position> sensor_and_closest_beacon;
    while (ss >> sensor_and_closest_beacon) {
        sensors.emplace_back(std::move(sensor_and_closest_beacon));
    }

    assert(PositionsThatCannotContainBeacon(sensors, 10) == 26);
    //assert(PositionsThatCannotContainBeacon(sensors, 10) == 26);

    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return Test();
    }

    return ParseAndRun(argv[1]);
}
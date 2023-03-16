#ifndef AOC_2022_DAY15_SENSORANDBEACONSMAP_H
#define AOC_2022_DAY15_SENSORANDBEACONSMAP_H

#include <cstdint>
#include <vector>
#include <utility>
#include <istream>
#include <sstream>

namespace AOC2022Day15SensorsAndBeaconsMap {
    struct Position final {
        int64_t x;
        int64_t y;

        // Get Manhattan/Taxicab distance
        int64_t Distance(const Position& other) const {
            return std::abs(x - other.x) + std::abs(y - other.y);
        }

        friend std::istream& operator>>(std::istream& stream, std::pair<Position, Position>& sensor_and_closest_beacon) {
            std::string line;
            std::getline(stream, line);

            if (line.empty()) {
                stream.setstate(std::ios_base::failbit);
                return stream;
            }

            {
                const auto colon_index = line.find(':');
                std::stringstream ss(line.substr(11, colon_index));

                char delimiter;
                ss >> delimiter;
                ss >> sensor_and_closest_beacon.first.x;
                ss >> delimiter >> delimiter >> delimiter;
                ss >> sensor_and_closest_beacon.first.y;

                line = line.substr(colon_index + 1);
            }

            {
                std::stringstream ss(line.substr(23));

                char delimiter;
                ss >> delimiter;
                ss >> sensor_and_closest_beacon.second.x;
                ss >> delimiter >> delimiter >> delimiter;
                ss >> sensor_and_closest_beacon.second.y;
            }

            return stream;
        }
    };

    // Key idea is to compute intervals (coverage) for each sensor for the row (y-coordinate) of interest. Then merge such intervals.
    // This will give us the complete and final coverage (places/cells) where a beacon CANNOT lie.
    struct Interval final {
    public:
        int64_t min;
        int64_t max;

        [[nodiscard]] bool Subsumes(const Interval& other) const {
            return (min <= other.min) && (max >= other.max);
        }
        [[nodiscard]] bool Overlaps(const Interval& other) const {
            return Subsumes(other) || ((max >= other.min) && (max <= other.max)) || ((min <= other.max) && (min >= other.min));
        }

        friend auto operator<=>(const Interval& a, const Interval& b) = default;

        [[nodiscard]] Interval Merge(const Interval& other) const {
            return Interval{std::min(min, other.min), std::max(max, other.max)};
        }

        [[nodiscard]] int64_t Length() const {
            return max - min;
        }
    };
}

#endif //AOC_2022_DAY15_SENSORANDBEACONSMAP_H

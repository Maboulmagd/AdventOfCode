#ifndef AOC_2022_DAY04_INTERVAL_H
#define AOC_2022_DAY04_INTERVAL_H

#include <cstdint>
#include <istream>

namespace AOC2022Day04Interval {
    class Interval final {
    public:
        int32_t min;
        int32_t max;

        [[nodiscard]] bool Subsumes(const Interval& other) const;
        [[nodiscard]] bool Overlaps(const Interval& other) const;

        // Friend function has to be inside header file
        friend std::istream& operator>>(std::istream& file_stream, Interval& interval) {
            char delimiter;
            file_stream >> interval.min >> delimiter >> interval.max;
            return file_stream;
        }
    };
}

#endif //AOC_2022_DAY04_INTERVAL_H

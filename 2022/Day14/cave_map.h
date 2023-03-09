#ifndef AOC_2022_DAY14_CAVE_MAP_H
#define AOC_2022_DAY14_CAVE_MAP_H

#include <vector>
#include <cstdint>
#include <string>

namespace AOC2022Day14CaveMap {

    class CaveMap {
    public:
        // Our cave boundaries
        int32_t min_row_;
        int32_t max_row_;
        int32_t min_col_;
        int32_t max_col_;

        void UpdateBoundingBox(const int32_t row, const int32_t col);
        void Reset();

        std::vector<std::vector<char>> map_ = std::vector<std::vector<char>>(1'000, std::vector<char>(1'000, '.'));

        // We don't know the exact rows or columns of our cave, so we'll have to continuously update it as we read each scan
        void AddScan(const std::string& line);
        bool WithinCaveBounds(const int32_t min, const int32_t row) const;
    };

} // AOC2022Day14CaveMap

#endif //AOC_2022_DAY14_CAVE_MAP_H

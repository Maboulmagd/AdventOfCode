#ifndef AOC_2022_DAY12_HEIGHTMAP_H
#define AOC_2022_DAY12_HEIGHTMAP_H

#include <vector>
#include <cstdint>

namespace AOC2022Day12HeightMap {
    struct HeightMapPosition final {
        char val_;
        int32_t x_;
        int32_t y_;
        std::size_t num_steps_;

        HeightMapPosition Up() const {
            return HeightMapPosition{' ', x_ - 1, y_};
        }
        HeightMapPosition Down() const {
            return HeightMapPosition{' ', x_ + 1, y_};
        }
        HeightMapPosition Left() const {
            return HeightMapPosition{' ', x_, y_ - 1};
        }
        HeightMapPosition Right() const {
            return HeightMapPosition{' ', x_, y_ + 1};
        }
    };
}

#endif //AOC_2022_DAY12_HEIGHTMAP_H

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

        friend auto operator<=>(const HeightMapPosition&, const HeightMapPosition&) = default;// Required for unordered_set hashing
    };
}

// Explicit template specialization here...std::hash<Position>...also applies to all non-specializations...no?
// https://stackoverflow.com/questions/4872809/in-c-what-does-template-mean
template<> struct std::hash<AOC2022Day12HeightMap::HeightMapPosition> {
    std::size_t operator()(const AOC2022Day12HeightMap::HeightMapPosition& position) const {
        return std::hash<int>{}(position.x_) ^ std::hash<int>{}(position.y_ << 1uz) ^ position.val_;
    }
};

#endif //AOC_2022_DAY12_HEIGHTMAP_H

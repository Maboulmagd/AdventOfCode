#ifndef AOC_2022_DAY09_POSITION_H
#define AOC_2022_DAY09_POSITION_H

#include <cstdint>

namespace AOC2022Day09Position {
    struct Position final {
        // Not using uint32_t or std::size_t here, since we start at (0,0) and can go to negative positions
        int32_t row;
        int32_t col;

        explicit Position(int32_t row, int32_t col) : row(row), col(col) {}

        friend auto operator<=>(const Position&, const Position&) = default;
    };
}

// Explicit template specialization here...std::hash<Position>...also applies to all non-specializations...no?
// https://stackoverflow.com/questions/4872809/in-c-what-does-template-mean
template<> struct std::hash<AOC2022Day09Position::Position> {
    std::size_t operator()(const AOC2022Day09Position::Position& position) const {
        return std::hash<int>{}(position.row) ^ std::hash<int>{}(position.col << 1uz);
    }
};

#endif //AOC_2022_DAY09_POSITION_H

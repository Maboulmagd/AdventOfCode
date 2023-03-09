#include "cave_map.h"

#include <sstream>
#include <ranges>

namespace AOC2022Day14CaveMap {

    void CaveMap::UpdateBoundingBox(const int32_t row, const int32_t col) {
        min_row_ = std::min(min_row_, row);
        min_col_ = std::min(min_col_, col);

        max_row_ = std::max(max_row_, row);
        max_col_ = std::max(max_col_, col);
    }

    void CaveMap::AddScan(const std::string &line) {
        std::stringstream ss(line);

        int32_t prev_row = -1;
        int32_t prev_col = -1;

        int32_t row;
        int32_t col;
        char delimiter;

        while (ss >> col >> delimiter >> row) {
            if (prev_row == row) {
                for (int c : std::views::iota(std::min(prev_col, col), std::max(prev_col, col) + 1)) {
                    map_[row][c] = '#';
                    UpdateBoundingBox(row, c);
                }
            }

            if (prev_col == col) {
                for (int r : std::views::iota(std::min(prev_row, row), std::max(prev_row, row) + 1)) {
                    map_[r][col] = '#';
                    UpdateBoundingBox(r, col);
                }
            }

            prev_row = row;
            prev_col = col;
            ss >> delimiter >> delimiter;// Consume "->"
        }
    }

    bool CaveMap::WithinCaveBounds(const int32_t row, const int32_t col) const {
        if (row <= max_row_ && (col >= min_col_ && col <= max_col_)) { return true; }
        return false;
    }

    void CaveMap::Reset() {
        for (auto r : std::views::iota(0, 1'000)) {
            for (auto c : std::views::iota(0, 1'000)) {
                if (map_[r][c] == 'O') {
                    map_[r][c] = '.';
                }
            }
        }
    }
} // AOC2022Day14CaveMap
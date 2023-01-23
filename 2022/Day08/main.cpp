#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cassert>
#include <ranges>

uint32_t GetNumberOfTreesVisibleFromOutsideGrid(const std::vector<std::string>& input_data) {
    const std::size_t rows = input_data.size();
    const std::size_t cols = input_data[0].size();

    if (rows == 0 || cols == 0) { return 0; }
    assert(rows > 0 && cols > 0);

    // TODO Try doing the below with a views transform
    std::vector<std::vector<uint32_t>> tree_heights_grid(input_data.size(), std::vector<uint32_t>(input_data[0].size()));
    std::size_t row = 0;
    for (const std::string& tree_row : input_data) {
        tree_heights_grid[row] = std::vector<uint32_t>();
        for (const char& c : tree_row) {
            tree_heights_grid[row].push_back(c - '0');
        }
        assert(tree_heights_grid[row].size() == cols);
        ++row;
    }

    // Key idea now is to be able to get the tallest tree height to the West, East, North, and South of any tree in our grid
    // NOTE This problem's part one is extremely similar to the Trapping Rain Water problem from LeetCode
    std::vector<std::vector<uint32_t>> tallest_tree_left = tree_heights_grid;
    std::vector<std::vector<uint32_t>> tallest_tree_up = tree_heights_grid;

    row = 1;// Reuse row variable
    for (; row < rows - 1; ++row) {
        for (std::size_t col = 1; col < cols - 1; ++col) {
            tallest_tree_left[row][col] = std::max(tallest_tree_left[row][col-1], tree_heights_grid[row][col-1]);
            tallest_tree_up[row][col] = std::max(tallest_tree_up[row-1][col], tree_heights_grid[row-1][col]);

            const uint32_t tallest_tree_to_left = tallest_tree_left[row][col];
            const uint32_t tallest_tree_to_up = tallest_tree_up[row][col];
            const uint32_t s = 5zu;
        }
    }

    std::vector<std::vector<uint32_t>> tallest_tree_right = tree_heights_grid;
    std::vector<std::vector<uint32_t>> tallest_tree_down = tree_heights_grid;

    row = rows - 2;// Reuse row variable
    for (; row > 0; --row) {
        for (std::size_t col = cols - 2; col > 0; --col) {
            tallest_tree_right[row][col] = std::max(tallest_tree_right[row][col+1], tree_heights_grid[row][col+1]);
            tallest_tree_down[row][col] = std::max(tallest_tree_down[row+1][col], tree_heights_grid[row+1][col]);

            const uint32_t tallest_tree_to_right = tallest_tree_right[row][col];
            const uint32_t tallest_tree_to_down = tallest_tree_down[row][col];
            const uint32_t s = 5zu;
        }
    }

    uint32_t visible_trees = (2 * (rows + cols)) - 4;// Count trees on edge as they are always visible from outside the grid

    row = 1;// Reuse row variable
    for (; row < rows - 1; ++row) {
        for(std::size_t col = 1; col < cols - 1; ++col) {
            const uint32_t tree_height = tree_heights_grid[row][col];

            const uint32_t tallest_tree_north = tallest_tree_up[row][col];
            const uint32_t tallest_tree_south = tallest_tree_down[row][col];
            const uint32_t tallest_tree_west = tallest_tree_left[row][col];
            const uint32_t tallest_tree_east = tallest_tree_right[row][col];

            if ((tallest_tree_north < tree_height) || (tallest_tree_south < tree_height) ||
            (tallest_tree_west < tree_height) || (tallest_tree_east < tree_height)) {
                ++visible_trees;
            }
        }
    }

    return visible_trees;
}

/*
30373
25512
65332
33549
35390
 */

std::size_t GetBestScenicScore(const std::vector<std::string>& input_data) {
    const std::size_t rows = input_data.size();
    const std::size_t cols = input_data[0].size();

    if (rows == 0 || cols == 0) { return 0; }
    assert(rows > 0 && cols > 0);

    std::vector<std::vector<std::size_t>> scores(rows, std::vector<std::size_t>(cols, 1));

    for (auto r : std::views::iota(1uz, input_data.size() - 1)) {
        // Left to right
        std::vector<std::size_t> blocked(10, 0);// Last index where a tree of given height was seen
        for (auto c : std::views::iota(1uz, input_data[r].size() - 1)) {
            const std::size_t curr_tree_height = input_data[r][c] - '0';
            scores[r][c] *= c - blocked[curr_tree_height];

            // This tree blocks any tree of same height or shorter
            for (auto i : std::views::iota(0uz, curr_tree_height + 1)) {
                blocked[i] = c;
            }
        }

        // Right to left
        blocked = std::vector<std::size_t>(10, input_data.size() - 1);
        for (auto c : std::views::iota(1uz, input_data[r].size() - 1) | std::views::reverse) {
            const std::size_t curr_tree_height = input_data[r][c] - '0';
            scores[r][c] *= blocked[curr_tree_height] - c;

            // This tree blocks any tree of same height or shorter
            for (auto i : std::views::iota(0uz, curr_tree_height + 1)) {
                blocked[i] = c;
            }
        }
    }

    std::size_t best_score = 1;

    // Same thing, but now from top to bottom
    for (auto c : std::views::iota(1uz, input_data[0].size() - 1)) {
        // Top to bottom
        std::vector<std::size_t> blocked(10, 0);
        for (auto r: std::views::iota(1uz, input_data.size())) {
            const std::size_t curr_tree_height = input_data[r][c] - '0';
            scores[r][c] *= r - blocked[curr_tree_height];

            for (auto i: std::views::iota(0uz, curr_tree_height + 1)) {
                blocked[i] = r;
            }
        }

        // Bottom to top
        blocked = std::vector<std::size_t>(10, input_data.size() - 1);
        for (auto r: std::views::iota(1uz, input_data.size() - 1) | std::views::reverse) {
            const std::size_t curr_tree_height = input_data[r][c] - '0';
            scores[r][c] *= blocked[curr_tree_height] - r;

            // Take max score
            best_score = std::max(best_score, scores[r][c]);

            for (auto i: std::views::iota(0uz, curr_tree_height + 1)) {
                blocked[i] = r;
            }
        }
    }

    return best_score;
}

int ParseAndRun(const std::string& path) {
    std::fstream file_stream(path);

    if (!file_stream.is_open()) {
        std::cerr << "Failed to open: " << std::quoted(path) << std::endl;
        return 1;
    }

    std::vector<std::string> input_data;
    std::string line;
    while(std::getline(file_stream, line)) {
        input_data.push_back(line);
    }

    std::cout << GetNumberOfTreesVisibleFromOutsideGrid(input_data) << std::endl;
    std::cout << GetBestScenicScore(input_data) << std::endl;

    return 0;
}

int Test() {
    std::vector<std::string> data{
            "30373",
            "25512",
            "65332",
            "33549",
            "35390"
    };

    assert(GetNumberOfTreesVisibleFromOutsideGrid(data) == 21);
    assert(GetBestScenicScore(data) == 8);

    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return Test();
    }
    return ParseAndRun(argv[1]);
}
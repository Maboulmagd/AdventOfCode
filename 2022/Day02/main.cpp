#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>
#include <ranges>
#include <numeric>

constexpr int FollowStrategyGuide(const std::vector<std::pair<char,char>>& strategy_guide) {
    const auto round_score = [](const auto& pair) {
        const int elf = pair.first - 'A';
        const int me = pair.second - 'X';

        int my_score = me + 1;
        if (me == elf) {
            my_score += 3;
        }
        else if (((me - elf + 3) % 3) == 1) {
            my_score += 6;
        }

        return my_score;
    };

    const auto scores = strategy_guide | std::views::transform(round_score) | std::views::common;
    return std::reduce(scores.begin(), scores.end());
}

int main() {
    std::fstream input_file("2022\\Day02\\input.txt");

    std::vector<std::pair<char, char>> input;

    if (!input_file.is_open()) {
        //std::cerr << "Failed to open " << std::quoted(path) << '\n';
        return 1;
    }

    std::pair<char, char> current_round;
    while (input_file >> current_round.first >> current_round.second) {
        input.push_back(current_round);
    }

    std::cout << FollowStrategyGuide(input) << std::endl;

    return 0;
}
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <cassert>
#include <utility>
#include <algorithm>
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

constexpr int FollowChoiceGuide(const std::vector<std::pair<char,char>>& strategy_guide) {
    const auto score = [](const auto& pair){
        const int elf = pair.first - 'A';// 'A' means rock, 'B' means paper, 'C' means scissors
        const int my_turn = pair.second - 'X';// 'X' means lose, 'Y' means draw, 'Z' means win

        int my_score = my_turn * 3;

        if (my_turn == 1) {// Draw, choose same as elf
            my_score += elf + 1;
        }
        else if (my_turn == 2) {// Win, choose better than elf
            my_score += ((elf + 1) % 3) + 1;
        }
        else {// Lose, choose worse than elf
            my_score += ((elf - 1 + 3) % 3) + 1;
        }

        return my_score;
    };

    const auto scores = strategy_guide | std::views::transform(score) | std::views::common;
    return std::reduce(scores.begin(), scores.end());
}

int Test() {
    std::vector<std::pair<char, char>> test_data{
            {'A','Y'},
            {'B','X'},
            {'C','Z'}
    };
    assert(FollowStrategyGuide(test_data) == 15);
    assert(FollowChoiceGuide(test_data) == 12);

    return 0;
}

int ParseAndRun(const std::string& path) {
    std::fstream input_file(path);

    std::vector<std::pair<char, char>> input;

    if (!input_file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << '\n';
        return 1;
    }

    std::pair<char, char> current_round;
    while (input_file >> current_round.first >> current_round.second) {
        input.push_back(current_round);
    }

    std::cout << FollowStrategyGuide(input) << std::endl;
    std::cout << FollowChoiceGuide(input) << std::endl;

    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return Test();
    }

    return ParseAndRun(argv[1]);
}
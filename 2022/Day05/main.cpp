#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ranges>
#include <numeric>
#include <cassert>
#include <utility>
#include <iomanip>
#include <stack>
#include <cmath>

struct Procedure final {
    int32_t num_crates;
    int32_t from_stack;
    int32_t to_stack;

    friend std::istream& operator>>(std::istream& input_stream, Procedure& procedure) {
        std::string word;
        input_stream >> word >> procedure.num_crates >> word >> procedure.from_stack >> word >> procedure.to_stack;
        return input_stream;
    }
};

std::vector<std::vector<char>> ParseStackArrangements(std::fstream& input_file) {
    std::vector<std::vector<char>> stacks;
    bool parsed = false;

    std::string line;
    while (!parsed && std::getline(input_file, line)) {
        for (std::size_t i = 0; i < line.length(); ++i) {
            if (std::isdigit(line[i])) {
                parsed = true;
                break;
            }

            if (!std::isupper(line[i])) { continue; }

            std::size_t which_stack = std::ceil((i + 1) / 4.0f);// Figure out which stack this character is on

            // First stack (0th stack) should be empty...I know we are wasting some space there, but it is minimal
            if (which_stack + 1 > stacks.size()) {
                stacks.resize(which_stack + 1);
            }

            stacks[which_stack].push_back(line[i]);
        }

        // Consume the empty line
        if (parsed) {
            std::getline(input_file, line);
        }
    }

    assert(stacks[0].empty());
    std::ranges::for_each(stacks, std::ranges::reverse);// Reverse stacks to represent true arrangement

    return stacks;
}

std::string CrateMover9000(std::vector<std::vector<char>>& stacks, const std::vector<Procedure>& procedures) {
    std::string stacks_top;
    for (const auto& procedure : procedures) {
        for (int i = 0; i < procedure.num_crates; ++i) {
            stacks[procedure.to_stack].push_back(stacks[procedure.from_stack].back());
            stacks[procedure.from_stack].pop_back();
        }
    }

    for (const auto& stack : stacks) {
        if (!stack.empty()) {
            stacks_top += stack.back();
        }
    }

    return stacks_top;
}

std::string CrateMover9001(std::vector<std::vector<char>>& stacks, const std::vector<Procedure>& procedures) {
    std::string stacks_top;
    for (const auto& procedure : procedures) {
        std::stack<char> intermediary_stack;
        for (int i = 0; i < procedure.num_crates; ++i) {
            intermediary_stack.push(stacks[procedure.from_stack].back());
            stacks[procedure.from_stack].pop_back();
        }

        while (!intermediary_stack.empty()) {
            stacks[procedure.to_stack].push_back(intermediary_stack.top());
            intermediary_stack.pop();
        }
    }

    for (const auto& stack : stacks) {
        if (!stack.empty()) {
            stacks_top += stack.back();
        }
    }

    return stacks_top;
}

int ParseAndRun(const std::string path) {
    std::fstream input_file(path);

    if (!input_file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }

    std::vector<std::vector<char>> stacks = ParseStackArrangements(input_file);

    std::vector<std::vector<char>> stacks_copy = stacks;// Need a copy for part 2

    // Parse the rearrangement procedures
    std::vector<Procedure> procedures;

    std::ranges::copy(std::views::istream<Procedure>(input_file), std::back_inserter(procedures));

    std::cout << CrateMover9000(stacks, procedures) << std::endl;
    std::cout << CrateMover9001(stacks_copy, procedures) << std::endl;

    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return 1;
    }

    return ParseAndRun(argv[1]);
}
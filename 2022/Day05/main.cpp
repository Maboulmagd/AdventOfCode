#include "procedure.h"

#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <utility>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <ranges>
#include <cmath>

using namespace AOC2022Day05Procedure;

std::vector<std::vector<char>> ParseStackArrangements(std::istream& input_file) {
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
    for (const auto& procedure : procedures) {
        // view of the procedure's number of crates in reverse order
        const auto src = stacks[procedure.from_stack] | std::views::reverse | std::views::take(procedure.num_crates);
        std::ranges::copy(src, std::back_inserter(stacks[procedure.to_stack]));

        // Remove number of crates from procedure
        stacks[procedure.from_stack].resize(stacks[procedure.from_stack].size() - procedure.num_crates);
    }

    std::string stacks_top;
    for (const auto& stack : stacks) {
        if (!stack.empty()) {
            stacks_top += stack.back();
        }
    }

    return stacks_top;
}

std::string CrateMover9001(std::vector<std::vector<char>>& stacks, const std::vector<Procedure>& procedures) {
    for (const auto& procedure : procedures) {
        // view of the procedure's number of crates in reverse order
        const auto src = stacks[procedure.from_stack] | std::views::reverse
                | std::views::take(procedure.num_crates) | std::views::reverse;// Same as above, but need to reverse num crates selected
        std::ranges::copy(src, std::back_inserter(stacks[procedure.to_stack]));

        // Remove number of crates from procedure
        stacks[procedure.from_stack].resize(stacks[procedure.from_stack].size() - procedure.num_crates);
    }

    std::string stacks_top;
    for (const auto& stack : stacks) {
        if (!stack.empty()) {
            stacks_top += stack.back();
        }
    }

    return stacks_top;
}

int Test() {
    std::stringstream s(R"(    [D]
[N] [C]
[Z] [M] [P]
 1   2   3

move 1 from 2 to 1
move 3 from 1 to 3
move 2 from 2 to 1
move 1 from 1 to 2)");

    auto state = ParseStackArrangements(s);
    assert(state.size() == 4);
    auto l1 = {'Z', 'N'};
    assert(std::ranges::equal(state[1], l1));
    auto l2 = {'M', 'C', 'D'};
    assert(std::ranges::equal(state[2], l2));
    auto l3 = {'P'};
    assert(std::ranges::equal(state[3], l3));

    std::vector<Procedure> procedures;
    std::ranges::copy(std::views::istream<Procedure>(s), std::back_inserter(procedures));
    assert(procedures.size() == 4);
    auto cmp = {
            Procedure{1, 2, 1},
            Procedure{3, 1, 3},
            Procedure{2, 2, 1},
            Procedure{1, 1, 2}};
    assert(std::ranges::equal(procedures, cmp));

    auto state_copy = state;
    assert(CrateMover9000(state, procedures) == "CMZ");
    assert(CrateMover9001(state_copy, procedures) == "MCD");

    return 0;
}

int ParseAndRun(const std::string& path) {
    std::fstream input_file(path);

    if (!input_file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }

    std::vector<std::vector<char>> stacks = ParseStackArrangements(input_file);

    std::vector<std::vector<char>> stacks_copy = stacks;// Need a copy for part 2, since we modify stacks in-place

    // Parse the rearrangement procedures
    std::vector<Procedure> procedures;

    std::ranges::copy(std::views::istream<Procedure>(input_file), std::back_inserter(procedures));

    std::cout << CrateMover9000(stacks, procedures) << std::endl;
    std::cout << CrateMover9001(stacks_copy, procedures) << std::endl;

    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return Test();
    }

    return ParseAndRun(argv[1]);
}
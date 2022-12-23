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
    //std::ranges::for_each(stacks, std::ranges::reverse);// Reverse stacks to represent true arrangement

    return stacks;
}

std::string MoveCrates(std::vector<std::vector<char>>& stacks, const std::vector<Procedure>& procedures) {

    return std::to_string(stacks.size()) + " " + std::to_string(procedures.size());
}

int main() {
    std::fstream input_file("input.txt");

    if (!input_file.is_open()) {
        //std::cerr << "Failed to open " << std::quoted(input_file) << "\n";
        return 1;
    }

    std::vector<std::vector<char>> stacks = ParseStackArrangements(input_file);

    std::vector<std::vector<char>> stacks_copy = stacks;// Need a copy for part 2x

    // Parse the rearrangement procedures
    std::vector<Procedure> procedures;

    //std::ranges::copy(std::views::istream<Procedure>(input_file), std::back_inserter(procedures));

    std::cout << MoveCrates(stacks, procedures) << std::endl;
    //std::cout <<

    return 0;
}
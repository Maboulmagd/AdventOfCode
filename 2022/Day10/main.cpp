#include "cpu.h"

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <ranges>
#include <cassert>

using namespace AOC2022Day10CPUINSTRUCTION;
using namespace AOC2022Day10CPU;

int ParseAndRun(const std::string& path) {
    std::fstream file_stream(path);

    if (!file_stream.is_open()) {
        std::cerr << "Failed to open: " << std::quoted(path) << std::endl;
        return 1;
    }

    std::vector<CPUInstruction> instructions;
    std::ranges::copy(std::views::istream<CPUInstruction>(file_stream), std::back_inserter(instructions));
    assert(!instructions.empty());

    std::vector<int32_t> breakpoints = {20, 60, 100, 140, 180, 220};

    CPU* cpu = new CPU();

    std::cout << cpu->RunProgram(instructions, breakpoints) << std::endl;
    for (const auto& line : cpu->screen_) {
        std::cout << line << "\n";
    }

    delete cpu;

    return 0;
}

int Test() {
    std::stringstream s(R"(addx 15
addx -11
addx 6
addx -3
addx 5
addx -1
addx -8
addx 13
addx 4
noop
addx -1
addx 5
addx -1
addx 5
addx -1
addx 5
addx -1
addx 5
addx -1
addx -35
addx 1
addx 24
addx -19
addx 1
addx 16
addx -11
noop
noop
addx 21
addx -15
noop
noop
addx -3
addx 9
addx 1
addx -3
addx 8
addx 1
addx 5
noop
noop
noop
noop
noop
addx -36
noop
addx 1
addx 7
noop
noop
noop
addx 2
addx 6
noop
noop
noop
noop
noop
addx 1
noop
noop
addx 7
addx 1
noop
addx -13
addx 13
addx 7
noop
addx 1
addx -33
noop
noop
noop
addx 2
noop
noop
noop
addx 8
noop
addx -1
addx 2
addx 1
noop
addx 17
addx -9
addx 1
addx 1
addx -3
addx 11
noop
noop
addx 1
noop
addx 1
noop
noop
addx -13
addx -19
addx 1
addx 3
addx 26
addx -30
addx 12
addx -1
addx 3
addx 1
noop
noop
noop
addx -9
addx 18
addx 1
addx 2
noop
noop
addx 9
noop
noop
noop
addx -1
addx 2
addx -37
addx 1
addx 3
noop
addx 15
addx -21
addx 22
addx -6
addx 1
noop
addx 2
addx 1
noop
addx -10
noop
noop
addx 20
addx 1
addx 2
addx 2
addx -6
addx -11
noop
noop
noop)");

    std::vector<CPUInstruction> instructions;
    std::ranges::copy(std::views::istream<CPUInstruction>(s), std::back_inserter(instructions));
    assert(!instructions.empty());

    std::vector<int32_t> breakpoints = {20, 60, 100, 140, 180, 220};

    CPU cpu;

    assert(cpu.RunProgram(instructions, breakpoints) == 13140);
    for (const auto& line : cpu.screen_) {
        std::cout << line << "\n";
    }

    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return Test();
    }

    return ParseAndRun(argv[1]);
}
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <ranges>
#include <cassert>

#include "cpu_instruction.h"

using namespace AOC2022Day10CPUINSTRUCTION;

int32_t SignalStrengthSum(const std::vector<CPUInstruction>& instructions, const std::vector<int32_t>& breakpoints) {
    int32_t x_register = 1;
    int32_t signal_strength = 0;
    std::size_t cycle = 0uz;
    std::size_t breakpoints_i = 0uz;

    const auto parse_instructions = [&](const CPUInstruction& cpu_instruction) {
        const Instruction instruction = cpu_instruction.instruction_;
        switch (instruction) {
            case Instruction::ADDX:
                if (cycle + 2 >= breakpoints[breakpoints_i]) {
                    signal_strength += x_register * breakpoints[breakpoints_i];
                    ++breakpoints_i;
                }
                cycle += 2;
                x_register += cpu_instruction.value_.value();
                break;
            case Instruction::NOOP:
                if (cycle == breakpoints[breakpoints_i]) {
                    signal_strength += x_register * breakpoints[breakpoints_i];
                    ++breakpoints_i;
                }
                cycle += 1;
                break;
            default:
                throw std::runtime_error("Cannot parse CPUInstruction, invalid instruction type");
        }
        if (breakpoints_i > breakpoints.size()) {// Early break
            return;
        }
    };

    std::ranges::for_each(instructions, parse_instructions);
    return signal_strength;
}

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

    std::cout << SignalStrengthSum(instructions, breakpoints) << std::endl;
    //std::cout << SixSignalStrengthSum(instructions) << std::endl;

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

    assert(SignalStrengthSum(instructions, breakpoints) == 13140);

    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return Test();
    }

    return ParseAndRun(argv[1]);
}
#include <stdexcept>
#include <algorithm>
#include <cassert>

#include "cpu.h"

int32_t AOC2022Day10CPU::CPU::RunProgram(const std::vector<CPUInstruction>& instructions, const std::vector<int32_t>& breakpoints) {
    // Reset CPU fields (registers, signal strength, etc...)
    x_register_ = 1;
    signal_strength_ = 0;
    screen_.clear();

    auto breakpoints_it = breakpoints.begin();
    int32_t ticks = 0uz;// ticks, or cpu cycles

    for (CPUInstruction instruction : instructions) {// Iterate over instructions via copy, so as not to change input
        do {
            ++ticks;

            if (breakpoints_it != breakpoints.end() && ticks == *breakpoints_it) {
                signal_strength_ += x_register_ * ticks;
                ++breakpoints_it;
            }

            const int32_t sprite_position = (ticks - 1) % 40;
            if (sprite_position == 0) {// Add a new string (new line)
                screen_.emplace_back("");
            }

            if (sprite_position >= x_register_ - 1 && sprite_position <= x_register_ + 1) {// Sprite is visible
                screen_.back().push_back('#');
            }
            else {
                screen_.back().push_back('.');
            }

            if (instruction.instruction_ == Instruction::ADDX && instruction.phase_ == 1) {
                x_register_ += instruction.value_.value();
            }
        } while (instruction.Tick() != InstructionStatus::COMPLETE);
        assert(instruction.status_ == InstructionStatus::COMPLETE);
    }

    return signal_strength_;
}

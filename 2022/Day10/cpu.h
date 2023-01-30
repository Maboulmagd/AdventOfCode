#ifndef AOC_2022_DAY10_CPU_H
#define AOC_2022_DAY10_CPU_H

#include "cpu_instruction.h"

#include <vector>

using namespace AOC2022Day10CPUINSTRUCTION;

namespace AOC2022Day10CPU {
    class CPU final {
    public:
        int32_t x_register_;
        int32_t signal_strength_;
        std::vector<std::string> screen_;// Display

        int32_t RunProgram(const std::vector<CPUInstruction>& instructions, const std::vector<int32_t>& breakpoints);
    };

} // AOC2022Day10CPU

#endif //AOC_2022_DAY10_CPU_H

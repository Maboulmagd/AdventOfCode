#ifndef AOC_2022_DAY10_CPU_INSTRUCTION_H
#define AOC_2022_DAY10_CPU_INSTRUCTION_H

#include <optional>
#include <cstdint>

namespace AOC2022Day10CPUINSTRUCTION {
    enum class Instruction {
        ADDX,
        NOOP
    };

    struct CPUInstruction final {
        Instruction instruction_;
        std::optional<int32_t> value_;

        friend std::istream& operator>>(std::istream& stream, CPUInstruction& cpu_instruction) {
            cpu_instruction.instruction_ = Instruction::NOOP;
            cpu_instruction.value_ = std::nullopt;

            std::string instruction;
            stream >> instruction;
            if (instruction == "addx") {
                cpu_instruction.instruction_ = Instruction::ADDX;
                int32_t value;
                stream >> value;
                cpu_instruction.value_ = value;
            }

            return stream;
        }
    };
}

#endif //AOC_2022_DAY10_CPU_INSTRUCTION_H

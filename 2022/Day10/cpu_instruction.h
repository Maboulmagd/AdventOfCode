#ifndef AOC_2022_DAY10_CPU_INSTRUCTION_H
#define AOC_2022_DAY10_CPU_INSTRUCTION_H

#include <optional>
#include <cstdint>
#include <istream>

namespace AOC2022Day10CPUINSTRUCTION {
    enum class Instruction {
        ADDX,
        NOOP
    };

    enum class InstructionStatus {
        IN_PROGRESS,
        COMPLETE
    };

    class CPUInstruction final {
    public:
        Instruction instruction_;
        std::optional<int32_t> value_;
        int32_t phase_;
        InstructionStatus status_;

        InstructionStatus Tick() {
            ++phase_;

            if (instruction_ == Instruction::NOOP) {// Completed NOOP instruction
                status_ = InstructionStatus::COMPLETE;
            }
            else if (phase_ == 2){// Completed ADDX instruction
                status_ = InstructionStatus::COMPLETE;
            }

            return status_;
        }

        // Essentially this acts as our constructor
        friend std::istream& operator>>(std::istream& stream, CPUInstruction& cpu_instruction) {
            cpu_instruction.instruction_ = Instruction::NOOP;
            cpu_instruction.value_ = std::nullopt;
            cpu_instruction.phase_ = 0uz;
            cpu_instruction.status_ = InstructionStatus::IN_PROGRESS;

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

#ifndef AOC_2022_DAY05_PROCEDURE_H
#define AOC_2022_DAY05_PROCEDURE_H

#include <cstdint>
#include <istream>

namespace AOC2022Day05Procedure {
    class Procedure final {
    public:
        uint32_t num_crates;
        std::size_t from_stack;
        std::size_t to_stack;

        // Default implementation for spaceship comparison operator
        friend auto operator<=>(const Procedure&, const Procedure&) = default;
        // Friend function has to be inside header file
        friend std::istream& operator>>(std::istream& input_stream, Procedure& procedure) {
            std::string word;
            input_stream >> word >> procedure.num_crates >> word >> procedure.from_stack >> word >> procedure.to_stack;
            return input_stream;
        }
    };
}

#endif //AOC_2022_DAY05_PROCEDURE_H

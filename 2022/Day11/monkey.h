#ifndef AOC_2022_DAY11_MONKEY_H
#define AOC_2022_DAY11_MONKEY_H

#include <vector>
#include <cstdint>
#include <istream>
#include <sstream>
#include <cassert>
#include <iostream>
#include <limits>

namespace AOC2022Day11Monkey {
    struct Operation final {
        char operator_;
        int32_t operand_;
    };

    class Monkey final {
    public:
        uint32_t monkey_num_;
        std::vector<int64_t> items_and_worry_level_;
        Operation operation_;
        bool operation_operand_is_old_ = false;
        Operation test_;
        uint32_t if_true_monkey_;
        uint32_t if_false_monkey_;

        int64_t InspectItem(const int64_t item);
        uint32_t TestItem(const int64_t item);

        friend std::istream& operator>>(std::istream& stream, Monkey& monkey) {
            std::string line;

            // Get first line, or monkey number
            {
                std::getline(stream, line);
                std::stringstream ss(line.substr(7));
                uint32_t monkey_num;
                ss >> monkey_num;
                monkey.monkey_num_ = monkey_num;
            }

            // Get second line, or starting items
            {
                std::getline(stream, line);
                std::stringstream ss(line.substr(18));
                int64_t item;
                while (ss >> item) {
                    //std::cout << "Extracted item: " << item << std::endl;
                    monkey.items_and_worry_level_.push_back(item);
                    if (ss.peek() == ',') {
                        ss.ignore();
                    }
                }
            }

            // Get third line, or operation
            {
                std::getline(stream, line);
                std::stringstream ss(line.substr(23));// Extract "* 12", basically the operation and the operand
                if (std::isdigit(ss.str().back())) {// We have an integer
                    char operation;
                    int32_t operand;
                    ss >> operation >> operand;
                    monkey.operation_ = Operation{operation, operand};
                }
                else {
                    char operation;
                    ss >> operation;
                    monkey.operation_operand_is_old_ = true;
                    monkey.operation_ = Operation{operation, -1};
                }
            }

            // Get fourth line, or test operation
            {
                std::getline(stream, line);
                std::stringstream ss(line.substr(21));
                int32_t operand;
                ss >> operand;
                monkey.test_ = Operation{'/', operand};
            }

            // Get fifth line, or If true
            {
                std::getline(stream, line);
                std::stringstream ss(line.substr(29));
                uint32_t throw_to_monkey;
                ss >> throw_to_monkey;
                monkey.if_true_monkey_ = throw_to_monkey;
            }

            // Get sixth line, of If false
            {
                std::getline(stream, line);
                std::stringstream ss(line.substr(30));
                uint32_t throw_to_monkey;
                ss >> throw_to_monkey;
                monkey.if_false_monkey_ = throw_to_monkey;
            }

            if (!stream) {
                std::cerr << "File input stream failed 1 at Monkey number " << monkey.monkey_num_ << std::endl;
            }

            std::getline(stream, line);

            return stream;
        }
    };

} // AOC2022Day11Monkey

#endif //AOC_2022_DAY11_MONKEY_H

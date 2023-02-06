#include "monkey.h"

#include <fstream>
#include <iomanip>
#include <ranges>
#include <algorithm>

using namespace AOC2022Day11Monkey;

int64_t TwoMostActiveMonkeys(std::vector<Monkey> troop, uint32_t rounds = 20) {
    std::vector<int64_t> monkey_items_inspected(troop.size(), 0);

    for (int round = 0; round < rounds; ++round) {
        for (Monkey& monkey : troop) {
            const std::size_t monkey_items = monkey.items_and_worry_level_.size();
            monkey_items_inspected[monkey.monkey_num_] += monkey_items;

            for (int i = 0; i < monkey_items; ++i) {
                const int64_t old_item_worry_level = monkey.items_and_worry_level_[i];
                int64_t new_item_worry_level = monkey.InspectItem(old_item_worry_level);
                new_item_worry_level /= 3;

                const uint32_t monkey_to_throw_to = monkey.TestItem(new_item_worry_level);// Test which monkey to throw item to
                assert(monkey_to_throw_to != monkey.monkey_num_);

                troop.at(monkey_to_throw_to).items_and_worry_level_.push_back(new_item_worry_level);
            }

            monkey.items_and_worry_level_.clear();
        }
    }

    std::ranges::sort(monkey_items_inspected, std::greater<>{});
    return monkey_items_inspected[0] * monkey_items_inspected[1];
}

int ParseAndRun(const std::string& path) {
    std::fstream file_stream(path);

    if (!file_stream.is_open()) {
        std::cerr << "Failed to open: " << std::quoted(path) << std::endl;
        return 1;
    }

    std::vector<Monkey> troop;
    bool parsed = false;
    while (!parsed) {
        Monkey monkey;
        file_stream >> monkey;
        troop.push_back(monkey);
        if (troop.size() == 8) {
            parsed = true;
        }
    }
    assert(!troop.empty() && troop.size() == 8);

    std::cout << TwoMostActiveMonkeys(troop) << std::endl;

    return 0;
}

int Test() {
    std::vector<Monkey> troop;
    troop.push_back(Monkey{
            0,
            {79, 98},
            Operation{'*', 19},
            false,
            Operation{'/', 23},
            2,
            3
    });
    troop.push_back(Monkey{
            1,
            {54, 65, 75, 74},
            Operation{'+', 6},
            false,
            Operation{'/', 19},
            2,
            0
    });
    troop.push_back(Monkey{
            2,
            {79, 60, 97},
            Operation{'*', -1},
            true,
            Operation{'/', 13},
            1,
            3
    });
    troop.push_back(Monkey{
            3,
            {74},
            Operation{'+', 3},
            false,
            Operation{'/', 17},
            0,
            1
    });

    assert(TwoMostActiveMonkeys(troop) == 10605);

    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return Test();
    }

    return ParseAndRun(argv[1]);
}
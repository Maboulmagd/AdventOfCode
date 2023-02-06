#include "monkey.h"

#include <fstream>
#include <iomanip>
#include <ranges>
#include <algorithm>

using namespace AOC2022Day11Monkey;

std::pair<uint32_t, uint32_t> TwoMostActiveMonkeys(std::vector<Monkey> troop, uint32_t rounds = 20) {
    std::vector<uint32_t> monkey_items_inspected(troop.size(), 0uz);

    const auto monkey_business = [&troop, &monkey_items_inspected](Monkey& monkey) {
        monkey_items_inspected[monkey.monkey_num_] += monkey.items_and_worry_level_.size();
        const std::size_t num_items = monkey.items_and_worry_level_.size();

        for (int idx : std::views::iota(0uz, monkey.items_and_worry_level_.size())) {
            const uint32_t new_item_worry_level = monkey.InspectItem(idx);

            monkey.items_and_worry_level_[idx] = new_item_worry_level;
            monkey.items_and_worry_level_[idx] /= 3;// Divide worry level by 3

            const uint32_t monkey_to_throw_to = monkey.TestItem(idx);// Test which monkey to throw item to
            assert(monkey_to_throw_to != monkey.monkey_num_);

            troop[monkey_to_throw_to].items_and_worry_level_.push_back(monkey.items_and_worry_level_[idx]);
        }
        monkey.items_and_worry_level_.clear();
    };

    for ([[maybe_unused]] std::weakly_incrementable auto round : std::views::iota(0uz, rounds)) {
        std::ranges::for_each(troop, monkey_business);
    }

    std::ranges::sort(monkey_items_inspected);
    return std::make_pair(monkey_items_inspected[troop.size() - 2], monkey_items_inspected[troop.size() - 1]);
}

int ParseAndRun(const std::string& path) {
    std::fstream file_stream(path);

    if (!file_stream.is_open()) {
        std::cerr << "Failed to open: " << std::quoted(path) << std::endl;
        return 1;
    }

    std::vector<Monkey> troop;
    std::ranges::copy(std::views::istream<Monkey>(file_stream), std::back_inserter(troop));
    assert(!troop.empty());

    const std::pair<uint32_t, uint32_t> two_most_active_monkeys {TwoMostActiveMonkeys(troop)};
    std::cout << two_most_active_monkeys.first << two_most_active_monkeys.second << std::endl;
    //std::cout << two_most_active_monkeys.first << two_most_active_monkeys.second << std::endl;

    return 0;
}

int Test() {
    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return Test();
    }

    return ParseAndRun(argv[1]);
}
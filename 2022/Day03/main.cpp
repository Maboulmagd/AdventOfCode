#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <ranges>
#include <numeric>
#include <unordered_set>

constexpr int GetPrioritySum(const std::vector<std::string>& input) {
    const auto get_priority = [](const auto& line){
        assert(line.length() % 2 == 0);

        const size_t num_items = line.length();

        const std::string& first_compartment = line.substr(0, num_items / 2);
        const std::string& second_compartment = line.substr(num_items / 2);

        assert(first_compartment.length() == second_compartment.length());

        std::unordered_set<char> found;
        found.reserve(first_compartment.length());

        for (const char& c : first_compartment) {
            found.insert(c);
        }

        char common = '\0';

        for (const char& c : second_compartment) {
            if (found.find(c) != found.cend()) {
                assert(common == '\0');
                common = c;
                break;
            }
        }

        if (std::islower(common)) {
            return common - 'a' + 1;
        }

        return common - 'A' + 27;
    };

    const auto priorities = input | std::views::transform(get_priority) | std::views::common;
    return std::reduce(priorities.begin(), priorities.end());
}

constexpr int GetPrioritySumTriplets(const std::vector<std::string>& input) {
    assert(input.size() % 3 == 0);

    std::vector input_copy {input};

    // std::ranges::set_intersection requires input arrays to be sorted
    std::ranges::for_each(input_copy, std::ranges::sort);

    const std::size_t input_copy_size = input_copy.size();

    int priority_score_sum = 0;

    for (std::size_t i = 0; i + 2 < input_copy_size; i += 3) {
        // Get the common elements (set intersection) of the first 2 rucksacks
        std::vector<char> out;
        std::ranges::set_intersection(input_copy[i], input_copy[i + 1], std::back_inserter(out));

        assert(!out.empty());

        // Now get the common elements of out and the last (third) rucksack
        std::vector<char> res;
        std::ranges::set_intersection(out, input_copy[i + 2], std::back_inserter(res));

        // Get rid of extra copies using std::unique
        const auto last {std::unique(res.begin(), res.end())};
        res.erase(last, res.end());

        assert(res.size() == 1);

        const char& common {res.back()};
        if (std::islower(common)) {
            priority_score_sum += common - 'a' + 1;
        }
        else {
            priority_score_sum += common - 'A' + 27;
        }
    }

    return priority_score_sum;
}


int Test() {
    std::vector<std::string> test_data{
            "vJrwpWtwJgWrhcsFMMfFFhFp",
            "jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL",
            "PmmdzqPrVvPwwTWBwg",
            "wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn",
            "ttgJtRGJQctTZtZT",
            "CrZsJsPPZsGzwwsLwLmpwMDw",
    };
    assert(GetPrioritySum(test_data) == 157);
    assert(GetPrioritySumTriplets(test_data) == 70);

    return 0;
}

int ParseAndRun(const std::string& path) {
    std::fstream input_file(path);

    std::vector<std::string> input;

    if (!input_file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << '\n';
        return 1;
    }

    std::string line;
    while (std::getline(input_file, line)) {
        input.push_back(line);
    }

    std::cout << GetPrioritySum(input) << std::endl;
    std::cout << GetPrioritySumTriplets(input) << std::endl;

    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return Test();
    }

    return ParseAndRun(argv[1]);
}
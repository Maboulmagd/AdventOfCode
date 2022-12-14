#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ranges>
#include <numeric>
#include <cassert>
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

int main() {
    std::fstream input_file("input.txt");

    std::vector<std::string> input;

    if (!input_file.is_open()) {
        //std::cerr << "Failed to open " << std::quoted(path) << '\n';
        return 1;
    }

    std::string line;
    while (std::getline(input_file, line)) {
        input.push_back(line);
    }

    std::cout << GetPrioritySum(input) << std::endl;


    return 0;
}
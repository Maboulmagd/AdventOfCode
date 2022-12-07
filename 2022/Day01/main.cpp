#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ranges>
#include <numeric>

constexpr int GetMaxCalories(const std::vector<int>& calories_by_elf) {
    return *max_element(calories_by_elf.cbegin(), calories_by_elf.cend());
}

int GetTopThreeElvesCalories(const std::vector<int>& calories_by_elf) {
    std::vector<int> top_three(3);
    std::partial_sort_copy(calories_by_elf.begin(), calories_by_elf.end(), top_three.begin(), top_three.end(), std::greater<int>{});
    return std::reduce(top_three.cbegin(), top_three.cend());
}

int main() {
    std::fstream input_file("input.txt");

    std::vector<int> calories_by_elf;
    int curr_elf_calories = 0;

    std::string line;
    while (std::getline(input_file, line)) {
        if (line.empty()) {
            calories_by_elf.push_back(curr_elf_calories);
            curr_elf_calories = 0;
        }
        else {
            curr_elf_calories += stoi(line);
        }
    }

    std::cout << GetMaxCalories(calories_by_elf) << std::endl;
    std::cout << GetTopThreeElvesCalories(calories_by_elf) << std::endl;

    return 0;
}
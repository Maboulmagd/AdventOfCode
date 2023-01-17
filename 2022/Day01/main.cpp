#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <ranges>

constexpr uint64_t GetMaxCalories(const std::vector<std::string>& data) {
    const auto calories_by_elf = data |
            std::views::lazy_split(std::string{}) |// Splits container's data on empty string (empty line from input), as we want to group by elf
            std::views::transform([](const auto& elf) -> uint64_t {// Sum up the calories for each elf
                const auto to_unsigned = [](const std::string& in) { return std::stoull(in); };
                const auto rng = elf |
                        std::views::transform(to_unsigned) |
                        std::views::common;
                return std::reduce(rng.begin(), rng.end());
            });
    return std::ranges::max(calories_by_elf);
}

constexpr uint64_t GetTopThreeElvesCalories(const std::vector<std::string>& data) {
    const auto calories_by_elf = data |
            std::views::lazy_split(std::string{}) |// Splits container's data on empty string (empty line from input), as we want to group by elf
            std::views::transform([](const auto& elf) -> uint64_t {// Sum up the calories for each elf
                const auto to_unsigned = [](const std::string& in) { return std::stoull(in); };
                const auto rng = elf |
                        std::views::transform(to_unsigned) |
                        std::views::common;
                    return std::reduce(rng.begin(), rng.end());
            });
    std::vector<uint64_t> top_three_elves(3);
    std::ranges::partial_sort_copy(calories_by_elf, top_three_elves, std::greater<>());
    return std::reduce(top_three_elves.cbegin(), top_three_elves.cend());
}

int Test() {
    std::vector<std::string> test_data{
            "1000", "2000", "3000", "",
            "4000", "",
            "5000", "6000", "",
            "7000", "8000", "9000", "",
            "10000"
    };
    assert(GetMaxCalories(test_data) == 24000);
    assert(GetTopThreeElvesCalories(test_data) == 45000);

    return 0;
}

int ParseAndRun(const std::string& path) {
    std::fstream input_file(path);

    if (!input_file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }

    std::vector<std::string> data;
    std::string line;
    while (std::getline(input_file, line)) {
        data.push_back(line);
    }

    std::cout << GetMaxCalories(data) << std::endl;
    std::cout << GetTopThreeElvesCalories(data) << std::endl;

    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return Test();
    }

    return ParseAndRun(argv[1]);
}
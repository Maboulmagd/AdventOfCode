#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <cassert>
#include <unordered_map>

 uint32_t GetNumberOfCharactersForFirstMarker(const std::vector<std::string>& input, const std::size_t marker = 4) {
    assert(input.size() == 1);// Sanity check
    const std::string_view data_stream = input[0];
    const std::size_t data_len = data_stream.length();

    std::unordered_map<char, uint32_t> char_freq;
    char_freq.reserve(26);

    std::size_t i = 0;
    std::size_t j = 0;
    for (; j < data_len; ++j) {
        char_freq[data_stream[j]]++;// Increment count/frequency

        if (j >= marker) {
            // Check if our range from i to j has unique characters (count is 1)
            bool has_unique = true;
            for (std::size_t m = i; m < j+1; ++m) {
                if (char_freq[data_stream[m]] != 1) {
                    has_unique = false;
                    break;
                }
            }

            if (has_unique) {
                break;
            }

            char_freq[data_stream[i]]--;// Decrement count/frequency of character leaving window
            ++i;
        }
    }

    return j;
}

int ParseAndRun(const std::string& path) {
    std::fstream file_name(path);

    if (!file_name.is_open()) {
        std::cerr << "Failed to open: " << std::quoted(path) << std::endl;
        return 1;
    }

    std::vector<std::string> input;

    std::string line;
    while (std::getline(file_name, line)) {
        input.push_back(line);
    }

    std::cout << GetNumberOfCharactersForFirstMarker(input) << '\n';// Start of packet marker
    std::cout << GetNumberOfCharactersForFirstMarker(input, 14) << '\n';// Start of message marker

    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return 1;
    }

    return ParseAndRun(argv[1]);
}
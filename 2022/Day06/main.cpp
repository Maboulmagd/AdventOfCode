#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <cassert>
#include <unordered_map>

 std::size_t GetNumberOfCharactersForFirstMarker(const std::string& input, const std::size_t marker = 4) {
    const std::size_t data_len = input.length();

    std::unordered_map<char, uint32_t> char_freq;
    char_freq.reserve(26);

    std::size_t i = 0;
    std::size_t j = 0;
    for (; j < data_len; ++j) {
        char_freq[input[j]]++;// Increment count/frequency

        if (j+1 >= marker) {
            if (char_freq.size() == marker) {
                break;
            }

            char_freq[input[i]]--;// Decrement count/frequency of character leaving window
            if (char_freq[input[i]] == 0) {// Remove character from our map, to reduce size
                char_freq.erase(input[i]);
            }

            ++i;
        }
    }

    return j+1;
}

int ParseAndRun(const std::string& path) {
    std::fstream file_name(path);

    if (!file_name.is_open()) {
        std::cerr << "Failed to open: " << std::quoted(path) << std::endl;
        return 1;
    }

    std::string input;
    std::getline(file_name, input);

    std::cout << GetNumberOfCharactersForFirstMarker(input) << '\n';// Start of packet marker
    std::cout << GetNumberOfCharactersForFirstMarker(input, 14) << '\n';// Start of message marker

    return 0;
}

int Test() {
    std::string t1 = "bvwbjplbgvbhsrlpgdmjqwftvncz"; // 5
    assert(GetNumberOfCharactersForFirstMarker(t1) == 5);
    assert(GetNumberOfCharactersForFirstMarker(t1, 14) == 23);
    std::string t2 = "nppdvjthqldpwncqszvftbrmjlhg"; // 6
    assert(GetNumberOfCharactersForFirstMarker(t2) == 6);
    assert(GetNumberOfCharactersForFirstMarker(t2, 14) == 23);
    std::string t3 = "nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg"; // 10
    assert(GetNumberOfCharactersForFirstMarker(t3) == 10);
    assert(GetNumberOfCharactersForFirstMarker(t3, 14) == 29);
    std::string t4 = "zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw"; // 11
    assert(GetNumberOfCharactersForFirstMarker(t4) == 11);
    assert(GetNumberOfCharactersForFirstMarker(t4, 14) == 26);

    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return Test();
    }

    return ParseAndRun(argv[1]);
}
#include "packet.h"

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <utility>
#include <sstream>
#include <cassert>

using namespace AOC2022Day13Packet;

uint32_t NumberOfPacketsInRightOrder(const std::vector<Packet>& packets) {
    uint32_t res = 0;
    int32_t index = 1;

    for (int32_t i = 1; i < packets.size(); i += 2) {
        if (packets[i - 1] < packets[i]) {
            res += index;
        }
        ++index;
    }

    return res;
}

int32_t FindDecoderKey(std::vector<Packet>& packets) {
    std::sort(packets.begin(), packets.end());

    const auto two_it = std::lower_bound(packets.cbegin(), packets.cend(), Packet{ 2 });
    const auto six_it = std::upper_bound(packets.cbegin(), packets.cend(), Packet{ 6 });

    const auto two_index = std::distance(packets.cbegin(), two_it) + 1;
    const auto six_index = std::distance(packets.cbegin(), six_it) + 2;// 2 because we have to pretend that 2 is added in there

    return two_index * six_index;
}

int ParseAndRun(const std::string& path) {
    std::fstream file_stream(path);

    if (!file_stream.is_open()) {
        std::cerr << "Failed to open: " << std::quoted(path) << std::endl;
        return 1;
    }

    std::vector<Packet> packets;
    Packet a;
    Packet b;
    while (file_stream >> a >> b) {
        packets.emplace_back(std::move(a));
        packets.emplace_back(std::move(b));
    }

    std::cout << NumberOfPacketsInRightOrder(packets) << std::endl;
    std::cout << FindDecoderKey(packets) << std::endl;

    return 0;
}

int Test() {
    std::stringstream ss(R"([1,1,3,1,1]
[1, 1, 5, 1, 1]
    
[[1], [2, 3, 4]]
[[1], 4]

[9]
[[8, 7, 6]]

[[4, 4], 4, 4]
[[4, 4], 4, 4, 4]

[7, 7, 7, 7]
[7, 7, 7]

[]
[3]

[[[]] ]
[[]]

[1, [2, [3, [4, [5, 6, 7]]]], 8, 9]
[1, [2, [3, [4, [5, 6, 0]]]], 8, 9])");

    std::vector<Packet> packets;
    Packet a;
    Packet b;
    while (ss >> a >> b) {
        packets.emplace_back(std::move(a));
        packets.emplace_back(std::move(b));
    }

    assert(NumberOfPacketsInRightOrder(packets) == 13);
    assert(FindDecoderKey(packets) == 140);

    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return Test();
    }

    return ParseAndRun(argv[1]);
}
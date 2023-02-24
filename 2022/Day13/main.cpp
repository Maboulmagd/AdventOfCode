#include "packet.h"

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <utility>
#include <sstream>
#include <cassert>

using namespace AOC2022Day13Packet;

uint32_t NumberOfPacketsInRightOrder(const std::vector<std::pair<Packet, Packet>>& packet_pairs) {
    uint32_t res = 0;
    uint32_t index = 1;

    for (const auto& packet_pair : packet_pairs) {
        if (packet_pair.first < packet_pair.second) {
            res += index;
        }
        ++index;
    }

    return res;
}

int ParseAndRun(const std::string& path) {
    std::fstream file_stream(path);

    if (!file_stream.is_open()) {
        std::cerr << "Failed to open: " << std::quoted(path) << std::endl;
        return 1;
    }

    std::vector<std::pair<Packet, Packet>> packet_pairs;
    Packet a;
    Packet b;
    while (file_stream >> a >> b) {
        packet_pairs.emplace_back(std::move(a), std::move(b));
    }

    std::cout << NumberOfPacketsInRightOrder(packet_pairs) << std::endl;

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

    std::vector<std::pair<Packet, Packet>> packet_pairs;
    Packet a;
    Packet b;
    while (ss >> a >> b) {
        packet_pairs.emplace_back(std::move(a), std::move(b));
    }

    assert(NumberOfPacketsInRightOrder(packet_pairs) == 13);

    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return Test();
    }

    return ParseAndRun(argv[1]);
}
#include "packet.h"

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <utility>

using namespace AOC2022Day13Packet;

int ParseAndRun(const std::string& path) {
    std::fstream file_stream(path);

    if (!file_stream.is_open()) {
        std::cerr << "Failed to open: " << std::quoted(path) << std::endl;
        return 1;
    }

    std::vector<std::pair<Packet, Packet>> packet_pairs_;
    Packet a;
    Packet b;
    while (file_stream >> a >> b) {
        packet_pairs_.emplace_back(std::move(a), std::move(b));
    }

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
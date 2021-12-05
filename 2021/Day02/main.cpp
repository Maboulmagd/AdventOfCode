#include <fstream>
#include <iostream>
#include <vector>
#include <utility>

void solve_a(const std::vector<std::pair<char, int>>& commands) {
    int horizontal = 0;
    int depth = 0;

    for (const auto& command : commands) {
        const char direction = command.first;
        const int magnitude = command.second;

        switch (direction) {
            case 'f':
                horizontal += magnitude;
                break;
            case 'u':
                depth -= magnitude;
                break;
            case 'd':
                depth += magnitude;
                break;
            default:
                break;
        }
    }
    
    std::cout << "Product of final horizontal and depth position: " << horizontal * depth << std::endl;
}

void solve_b(const std::vector<std::pair<char, int>>& commands) {
    int horizontal = 0;
    int depth = 0;
    int aim = 0;

    for (const auto& command : commands) {
        const char direction = command.first;
        const int magnitude = command.second;

        switch (direction) {
            case 'f':
                horizontal += magnitude;
                depth += aim * magnitude;
                break;
            case 'u':
                aim -= magnitude;
                break;
            case 'd':
                aim += magnitude;
                break;
            default:
                break;
        }
    }
    
    std::cout << "Product of AMMENDED final horizontal and depth position: " << horizontal * depth << std::endl;
}

int main() {
    std::ifstream input_file("input.txt");

    std::vector<std::pair<char, int>> res;

    std::string line;
    while (std::getline(input_file, line)) {
        std::size_t pos = line.find("forward");
        int magnitude = 0;

        if (pos != std::string::npos) {
            magnitude = std::stoi(line.substr(8));
            res.push_back(std::make_pair('f', magnitude));
        }

        pos = line.find("down");
        if (pos != std::string::npos) {
            magnitude = std::stoi(line.substr(5));
            res.push_back(std::make_pair('d', magnitude));
        }

        pos = line.find("up");
        if (pos != std::string::npos) {
            magnitude = std::stoi(line.substr(3));
            res.push_back(std::make_pair('u', magnitude));
        }
    }

    solve_a(res);
    solve_b(res);
    
    return 0;
}
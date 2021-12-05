#include <fstream>
#include <iostream>
#include <vector>

int main() {
    std::ifstream input_file("input.txt");

    std::vector<int> res;

    std::string line;
    while (std::getline(input_file, line)) {
        res.push_back(std::stoi(line));
    }

    // solve_a(res);
    // solve_b(res);
    
    return 0;
}
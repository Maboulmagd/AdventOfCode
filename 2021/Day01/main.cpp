#include <fstream>
#include <iostream>
#include <vector>

// Solution to Part A
void solve_a(const std::vector<int>& depth_measurements) {
    const std::size_t measurements = depth_measurements.size();
    int total_increases = 0;

    for (std::size_t i = 1; i < measurements; ++i) {
        if (depth_measurements[i] > depth_measurements[i-1]) {
            ++total_increases;
        }
    }
    
    std::cout << "Part A total increases: " << total_increases << std::endl;
}

// Solution to Part B
void solve_b(const std::vector<int>& depth_measurements) {
    const std::size_t measurements = depth_measurements.size();
    
    int total_window_increases = 0;
    // Key idea here is to maintain a sliding window of length 3
    
    // Compute sum of first window
    int prev_window_sum = 0;
    std::size_t i = 0;
    while (i < 3) {
        prev_window_sum += depth_measurements[i];
        ++i;
    }

    int total_windows = 1;
    // Now move sliding window by 1, and count total sum increases
    while (i < measurements) {
        const int curr_window_sum = prev_window_sum - depth_measurements[i - 3] + depth_measurements[i];// New window sum
        if (curr_window_sum > prev_window_sum) {
            ++total_window_increases;
        }
        prev_window_sum = curr_window_sum;
        ++i;
        ++total_windows;
    }

    std::cout << "Part B total_window_increases: " << total_window_increases << std::endl;
}

int main() {
    std::ifstream input_file("input.txt");

    std::vector<int> res;

    std::string line;
    while (std::getline(input_file, line)) {
        res.push_back(std::stoi(line));
    }

    solve_a(res);
    solve_b(res);
    
    return 0;
}
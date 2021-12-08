#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

// Solution to Part A
void SolveA(const vector<int>& depth_measurements) {
    const size_t measurements = depth_measurements.size();
    int total_increases = 0;

    for (size_t i = 1; i < measurements; ++i) {
        if (depth_measurements[i] > depth_measurements[i-1]) {
            ++total_increases;
        }
    }
    
    cout << "Part A total increases: " << total_increases << endl;
}

// Solution to Part B
void SolveB(const vector<int>& depth_measurements) {
    const size_t measurements = depth_measurements.size();
    
    int total_window_increases = 0;
    // Key idea here is to maintain a sliding window of length 3
    
    // Compute sum of first window
    int prev_window_sum = 0;
    size_t i = 0;
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

    cout << "Part B total_window_increases: " << total_window_increases << endl;
}

int main() {
    ifstream input_file("input.txt");

    vector<int> res;

    string line;
    while (getline(input_file, line)) {
        res.push_back(stoi(line));
    }

    SolveA(res);
    SolveB(res);
    
    return 0;
}
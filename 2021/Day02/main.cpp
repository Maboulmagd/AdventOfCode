#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <string>

using namespace std;

void SolveA(const vector<pair<char, int>>& commands) {
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
    
    cout << "Product of final horizontal and depth position: " << horizontal * depth << endl;
}

void SolveB(const vector<pair<char, int>>& commands) {
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
    
    cout << "Product of AMMENDED final horizontal and depth position: " << horizontal * depth << endl;
}

int main() {
    ifstream input_file("input.txt");

    vector<pair<char, int>> res;

    string line;
    while (getline(input_file, line)) {
        size_t pos = line.find("forward");
        int magnitude = 0;

        if (pos != string::npos) {
            magnitude = stoi(line.substr(8));
            res.push_back(make_pair('f', magnitude));
        }

        pos = line.find("down");
        if (pos != string::npos) {
            magnitude = stoi(line.substr(5));
            res.push_back(make_pair('d', magnitude));
        }

        pos = line.find("up");
        if (pos != string::npos) {
            magnitude = stoi(line.substr(3));
            res.push_back(make_pair('u', magnitude));
        }
    }

    SolveA(res);
    SolveB(res);
    
    return 0;
}
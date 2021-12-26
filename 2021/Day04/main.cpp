#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

struct Board final {
    vector<vector<int>> rows;
};

int ComputeSum(const vector<Board>& board) {
    int sum = 0;

    return sum;
}

void IsWinner(const vector<Board>& board) {

}

void SolveA(const vector<int> numbers_to_draw, vector<Board>& boards) {

}

void SolveB() {

}

int main() {
    ifstream input_file("input.txt");

    vector<int> numbers_to_draw;
    vector<Board> boards;

    string line;

    if (getline(input_file, line, ',')) {
        numbers_to_draw.push_back(stoi(line));
    }

    
        
    }

    SolveA(numbers_to_draw, boards);

    return 0;
}
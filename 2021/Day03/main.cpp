#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <math.h>

using namespace std;

int BinaryToDecimal(const string& input) {
    const int bits = static_cast<int>(input.length());

    int res = 0;
    double power = 0.0;

    for (int bit = bits - 1; bit >= 0; --bit) {
        if (input.at(bit) == '1') { res += static_cast<int>(pow(2.0, power)); }
        ++power;
    }

    return res;
}

void SolveA(const vector<string>& input) {
    const size_t bits = input[0].length();

    string gamma_rate(bits, '0');
    string epsilon_rate(bits, '0');

    int zeros = 0;
    int ones = 0;

    for (int bit = 0; bit < bits; ++bit) {
        for (const auto& str : input) {
            if (str[bit] == '0') { ++zeros; }
            else { ++ones; }
        }

        if (ones > zeros) {
            gamma_rate[bit] = '1';
            epsilon_rate[bit] = '0';
        }
        else {
            gamma_rate[bit] = '0';
            epsilon_rate[bit] = '1';
        }

        zeros ^= zeros;
        ones ^= ones;
    }

    const int power_consumption = BinaryToDecimal(gamma_rate) * BinaryToDecimal(epsilon_rate);

    cout << "gamma rate: " + gamma_rate + " epsilon rate: " + epsilon_rate + " power consumption of submarine: " + to_string(power_consumption) << endl;
}

const string FindOxygenGeneratorRating(const vector<string>& input) {
    const size_t bits = input[0].length();
    vector<bool> valid(input.size(), true);

    int zeros = 0;
    int ones = 0;

    int valid_count = static_cast<int>(input.size());

    string oxygen_generator_rating;

    int bit = 0;
    for (; bit < bits && valid_count > 2; ++bit) {

        int index = 0;
        for (const auto& str : input) {
            if (valid[index]) {
                if (str[bit] == '0') { ++zeros; }
                else { ++ones; }
            }
            ++index;
        }
        
        // Filter ratings
        if (ones >= zeros) {
            int index = 0;
            for (const auto& str : input) {
                if (valid[index]) {
                    if (str[bit] == '0') { 
                        valid[index] = false;
                        --valid_count;
                    }
                }
                ++index;
            }
        }

        else if (ones < zeros) {
            int index = 0;
            for (const auto& str : input) {
                if (valid[index]) {
                    if (str[bit] == '1') {
                        valid[index] = false;
                        --valid_count;
                    }
                }
                ++index;
            }
        }

        zeros ^= zeros;
        ones ^= ones;
    }

    if (valid_count == 2) {// 2 valid strings
        int index_one = -1;
        int index_two = -1;

        int index = 0;
        
        for (const auto& str : input) {
            if (valid[index]) {
                if (index_one == -1) { index_one = index; }
                else if (index_one != -1) { index_two = index; }
            }
            ++index;
        }

        if (input[index_one][bit] != input[index_two][bit]) {// 2 valid strings
            if (input[index_one][bit] == '1') {
                oxygen_generator_rating = input[index_one];
            }
            else {
                oxygen_generator_rating = input[index_two];
            }
        }
    }

    else {// Just 1 valid string
        int index = 0;
        for (const auto& str : input) {
            if (valid[index]) {
                break;
            }
            ++index;
        }

        oxygen_generator_rating = input[index];
    }

    return oxygen_generator_rating;
}

const string FindCO2ScrubbingRating(const vector<string>& input) {
    const size_t bits = input[0].length();
    vector<bool> valid(input.size(), true);

    int zeros = 0;
    int ones = 0;

    int valid_count = static_cast<int>(input.size());

    string co2_scrubbing_rating;

    int bit = 0;
    for (; bit < bits && valid_count > 2; ++bit) {

        int index = 0;
        for (const auto& str : input) {
            if (valid[index]) {
                if (str[bit] == '0') { ++zeros; }
                else { ++ones; }
            }
            ++index;
        }
        
        // Filter ratings
        if (ones >= zeros) {
            int index = 0;
            for (const auto& str : input) {
                if (valid[index]) {
                    if (str[bit] == '1') {
                        valid[index] = false;
                        --valid_count;
                    }
                }
                ++index;
            }
        }

        else if (ones < zeros) {
            int index = 0;
            for (const auto& str : input) {
                if (valid[index]) {
                    if (str[bit] == '0') {
                        valid[index] = false;
                        --valid_count;
                    }
                }
                ++index;
            }
        }

        zeros ^= zeros;
        ones ^= ones;
    }

    if (valid_count == 2) {// 2 valid strings
        int index_one = -1;
        int index_two = -1;

        int index = 0;
        
        for (const auto& str : input) {
            if (valid[index]) {
                if (index_one == -1) { index_one = index; }
                else if (index_one != -1) { index_two = index; }
            }
            ++index;
        }

        if (input[index_one][bit] != input[index_two][bit]) {// 2 valid strings
            if (input[index_one][bit] == '0') {
                co2_scrubbing_rating = input[index_one];
            }
            else {
                co2_scrubbing_rating = input[index_two];
            }
        }
    }
   
    else {// Just 1 valid string
        int index = 0;
        for (const auto& str : input) {
            if (valid[index]) {
                break;
            }
            ++index;
        }

        co2_scrubbing_rating = input[index];
    }

    return co2_scrubbing_rating;
}

void SolveB(const vector<string>& input) {
    const string oxygen_generator_rating = FindOxygenGeneratorRating(input);
    const string co2_scrubbing_rating = FindCO2ScrubbingRating(input);

    const int life_support_rating = BinaryToDecimal(oxygen_generator_rating) * BinaryToDecimal(co2_scrubbing_rating);

    cout << "oxygen generator rating: " + oxygen_generator_rating + " co2 scrubber rating: " + co2_scrubbing_rating 
         + " life support rating: " + to_string(life_support_rating) << endl;
}

int main() {
    vector<string> input;
    ifstream input_file("input.txt");

    string line;
    while (getline(input_file, line)) {
        input.push_back(line);
    }

    SolveA(input);
    SolveB(input);
    //SolveB( { "00100", "11110", "10110", "10111", "10101", "01111", "00111", "111000", "10000", "11001", "00010", "01010" } );

    return 0;
}
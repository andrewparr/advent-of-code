#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>

int main(int argc, char** argv) {

    std::vector<std::string> input;
    while (!std::cin.eof()) {
        std::string value;
        std::cin >> value;
        input.push_back(value);
    }

    std::vector<int> freq_0(input[0].length());
    std::vector<int> freq_1(input[0].length());
    for (std::string value : input) {
        for (size_t i = 0; i < value.length(); ++i) {
            if (value[i] == '0')
                freq_0[i]++;
            else if (value[i] == '1')
                freq_1[i]++;
        }
    }
    int gamma = 0;
    int epsilon = 0;
    for (size_t i = 0; i < freq_0.size(); ++i) {
        gamma <<= 1;
        epsilon <<= 1;
        if (freq_1[i] > freq_0[i])
            gamma |= 0x01;
        else
            epsilon |= 0x01;
    }
    std::cout << gamma * epsilon << std::endl;

    // oxygen
    std::vector<std::string> temp = input;
    int offset = 0;
    while (temp.size() > 1) {
        int count = count_if(temp.begin(), temp.end(), [offset](const std::string& s) {
           return s[offset] == '1';
        });
        char keep = (count<<1 >= temp.size()) ? '1' : '0';
        auto pend = remove_if(temp.begin(), temp.end(), [offset, keep](const std::string& s) {
           return s[offset] != keep;
        });
        temp.erase(pend, temp.end());
        offset++;
    }
    int oxygen = stoi(temp[0], nullptr, 2);

    // co2
    temp = input;
    offset = 0;
    while (temp.size() > 1) {
        int count = count_if(temp.begin(), temp.end(), [offset](const std::string& s) {
           return s[offset] == '1';
        });
        char keep = (count<<1 < temp.size()) ? '1' : '0';
        auto pend = remove_if(temp.begin(), temp.end(), [offset, keep](const std::string& s) {
           return s[offset] != keep;
        });
        temp.erase(pend, temp.end());
        offset++;
    }
    int co2 = stoi(temp[0], nullptr, 2);

    std::cout << oxygen * co2 << std::endl;

    return 0;
}
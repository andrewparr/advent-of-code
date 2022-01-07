#include <iostream>
#include <string>
// #include <vector>
// #include <array>
#include <map>
// #include <set>
// #include <queue>
#include <list>
// #include <algorithm>
// #include <utility>
// #include <assert.h>
// #include <cstring>
// #include <cctype>


int main(int argc, char** argv) {

    std::map<int, uint64_t> polymer;
    std::map<int, char> rules;

    std::string  line;
    std::cin >> line;
    for (size_t i = 0; i < line.length() - 1; ++i) {
        int ich = (line[i] << 8) + line[i+1];
        polymer[ich]++;
    }

    while (!std::cin.eof()) {
        std::string key, arrow, chstr;
        std::cin >> key >> arrow >> chstr;
        int i = (key[0] << 8) + key[1];
        char c = *chstr.rbegin();
        rules[i] = c;
    }

    // part 1
    for (int step = 0; step < 10; ++step) {
        std::map<int, uint64_t> new_polymer;
        for (auto a: polymer) {
            int i = a.first;
            char ch1 = (char)(a.first >> 8);
            char ch2 = (char)(a.first & 0xff);
            if (rules.count(i) > 0) {
                char c = rules[i];
                auto n = a.second;
                int left = (ch1 << 8) + c;
                int right = (c << 8) + (ch2);
                new_polymer[left] += n;
                new_polymer[right] += n;
            }
        }
        polymer = new_polymer;
    }

    std::map<char, uint64_t> cc;
    cc[line[0]]++; // add the first character which gets miscounted.
    for (auto a: polymer) {
        char ch1 = (char)(a.first >> 8);
        char ch2 = (char)(a.first & 0xff);
        cc[ch2] += a.second;
    }

    std::list<uint64_t> l;
    for (auto a : cc) {
        l.push_back(a.second);
    }
    l.sort();
    std::cout << *l.rbegin() - *l.begin() << std::endl;

    // part 2
    for (int step = 10; step < 40; ++step) {
        std::map<int, uint64_t> new_polymer;
        for (auto a: polymer) {
            int i = a.first;
            char ch1 = (char)(a.first >> 8);
            char ch2 = (char)(a.first & 0xff);
            if (rules.count(i) > 0) {
                char c = rules[i];
                auto n = a.second;
                int left = (ch1 << 8) + c;
                int right = (c << 8) + (ch2);
                new_polymer[left] += n;
                new_polymer[right] += n;
            }
        }
        polymer = new_polymer;
    }

    cc.erase(cc.begin(), cc.end());
    cc[line[0]]++; // add the first character which gets miscounted.
    for (auto a: polymer) {
        char ch1 = (char)(a.first >> 8);
        char ch2 = (char)(a.first & 0xff);
        cc[ch2] += a.second;
    }

    l.clear();
    for (auto a : cc) {
        l.push_back(a.second);
    }
    l.sort();
    std::cout << *l.rbegin() - *l.begin() << std::endl;

    return 0;
}
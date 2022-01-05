#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <algorithm>
#include <assert.h>

int main(int argc, char** argv) {

    std::vector<std::vector<std::string>> digits;
    std::vector<std::vector<std::string>> display;
    while (!std::cin.eof()) {
        std::string s;
        std::vector<std::string> l;
        for (int i = 0; i < 10; ++i) {
            std::cin >> s;
            sort(s.begin(), s.end());
            l.emplace_back(s);
        }
        digits.push_back(l);
        std::cin >> s;
        assert(s.compare("|") == 0);
        l.clear();
        for (int i = 0; i < 4; ++i) {
            std::cin >> s;
            sort(s.begin(), s.end());
            l.emplace_back(s);
        }
        display.push_back(l);
    }

    // for part_2 pre-processing
    std::vector<std::array<std::string,10>> keys;
    for (const auto& dd : digits) {
        std::map<size_t, std::vector<std::string>> m;
        for (const auto& b : dd) {
            m[b.size()].push_back(b);
        }
        //array<char,10> key;
        std::string zero;
        std::string one = m[2][0];
        std::string two;
        std::string three;
        std::string four =  m[4][0];
        std::string five;
        std::string six;
        std::string seven = m[3][0];
        std::string eight =  m[7][0];
        std::string nine;
        std::string ten;

        // two, three, five and nine have length 5
        // test for each one in turn.
        for (const auto& test : m[5]) {
            // out of 2,3 and 5
            // only 3 intersection with 1 is 1
            std::string temp;
            set_intersection(test.begin(), test.end(), one.begin(), one.end(), std::back_inserter(temp));
            if (temp.size() == 2) {
                three = test;
                break;
            }
        }

        // zero, six and nine have length 6.
        // test for each one in turn.
        for (const auto& test : m[6]) {
            // out of 0, 6 and 9
            // only 6 intersection with one will have length 1
            std::string temp;
            set_intersection(test.begin(), test.end(), one.begin(), one.end(), std::back_inserter(temp));
            if (temp.size() == 1) {
                six = test;
                continue;
            }
            // only 9 intersection with 4 will be 4
            temp.clear();
            set_intersection(test.begin(), test.end(), four.begin(), four.end(), std::back_inserter(temp));
            if (temp.compare(four) == 0) {
                nine = test;
                continue;
            }
        }
        // remaining one must be 0
        for (const auto& test : m[6]) {
            if (test == six)
                continue;
            if (test == nine)
                continue;
            zero = test;
        }

        // now have: 1,4,7,8,3,6 and 9

        // finding 2 or 5
        for (const auto& test : m[5]) {
            if (test == three)
                continue;
            std::string temp;
            // 5 union with 6 will be 6
            set_union(test.begin(), test.end(), six.begin(), six.end(), std::back_inserter(temp));
            if (temp.size() == 6) {
                five = test;
                continue;
            }
            // 2 intersection with 4 will be 2
            temp.clear();
            set_intersection(test.begin(), test.end(), four.begin(), four.end(), std::back_inserter(temp));
            if (temp.size() == 2) {
                two = test;
                continue;
            }

        }

        std::array<std::string,10> key;
        key[0] = zero;
        key[1] = one;
        key[2] = two;
        key[3] = three;
        key[4] = four;
        key[5] = five;
        key[6] = six;
        key[7] = seven;
        key[8] = eight;
        key[9] = nine;
        keys.emplace_back(key);
    }

    int part_1 = 0;
    int part_2 = 0;
    size_t i = 0;
    for (const auto& a : display) {
        int multiplier = 1000;
        int value = 0;
        const std::array<std::string,10>& key = keys[i++];
        for (auto b: a) {
            size_t len = b.length();
            if (len == 2 || len == 3 || len == 4 || len == 7) {
                part_1++;
            }
            sort(b.begin(), b.end());
            if (b == key[0]) {
                // nothing to do
            } else if (b == key[1]) {
                value += 1 * multiplier;
            } else if (b == key[2]) {
                value += 2 * multiplier;
            } else if (b == key[3]) {
                value += 3 * multiplier;
            } else if (b == key[4]) {
                value += 4 * multiplier;
            } else if (b == key[5]) {
                value += 5 * multiplier;
            } else if (b == key[6]) {
                value += 6 * multiplier;
            } else if (b == key[7]) {
                value += 7 * multiplier;
            } else if (b == key[8]) {
                value += 8 * multiplier;
            } else if (b == key[9]) {
                value += 9 * multiplier;
            }
            multiplier /= 10;
        }
        part_2 += value;
    }
    std::cout << part_1 << std::endl;
    std::cout << part_2 << std::endl;

    return 0;
}
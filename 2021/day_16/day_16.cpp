#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bitset>

long long vesion_count = 0;

long long process(const std::string& bits, size_t& idx, int num, int id) {
    std::vector<long long> values;
    int count = 1;
    while (idx < bits.length() - 6) {
        if (num > 0) {
            count++;
        }

        std::string str = bits.substr(idx, 3);
        idx += 3;
        int version = stoi(str, 0, 2);
        vesion_count += version;
        str = bits.substr(idx, 3);
        idx+=3;
        int id = stoi(str, 0, 2);
        if (id == 4) {
            // literal
            std::stringstream ss;
            do {
                str = bits.substr(idx, 5);
                idx+=5;
                ss << str.substr(1);
            } while (str[0] == '1');
            std::string temp = ss.str();
            long long literal = stoll(temp, 0, 2);
            values.push_back(literal);
        } else {
            if (bits[idx] == '0') {
                idx++;
                str = bits.substr(idx, 15);
                idx+=15;
                if (str.empty())
                    break;
                long length = stol(str, 0, 2);
                str = bits.substr(idx, length);
                size_t i = 0;
                long long ll = process(str, i, 0, id);
                idx += length;
                values.push_back(ll);
            } else {
                idx++;
                str = bits.substr(idx, 11);
                idx+=11;
                int num = stoi(str, 0, 2);
                long long ll = process(bits, idx, num, id);
                values.push_back(ll);
            }
        }
        if (num > 0) {
            num--;
            if (num == 0) {
                break;
            }
        }
    }
    long long value = 0;
    if (id == 0) {
        // sum
        for (auto a : values)
            value += a;
    } else if (id == 1) {
        // product
        value = 1;
        for (auto a : values)
            value *= a;
    } else if (id == 2) {
        // minimum
        value = values[0];
        for (auto a : values)
            if (a < value)
                value = a;
    } else if (id == 3) {
        // maximum
        value = values[0];
        for (auto a : values)
            if (a > value)
                value = a;
    } else if (id == 5) {
        // greater than
        value = values[0] > values[1] ? 1 : 0;
    } else if (id == 6) {
        // less than
        value = values[0] < values[1] ? 1 : 0;
    } else if (id == 7) {
        // equal to
        value = values[0] == values[1] ? 1 : 0;
    }
    return value;
}

int main(int argc, char** argv) {

    std::string hex_string;
    std::cin >> hex_string;

    std::stringstream ss;
    for (char c : hex_string) {
        ss << (c >= '0' && c <= '9' ? std::bitset<4>(c-'0') : std::bitset<4>(c-'A'+10));
    }
    std::string bits = ss.str();
    size_t idx = 0;
    long long ll = process(bits, idx, 0, 0);

    std::cout << vesion_count << std::endl;
    std::cout << ll << std::endl;

    return 0;
}
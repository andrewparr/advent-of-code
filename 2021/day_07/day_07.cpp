#include <iostream>
#include <vector>
#include <algorithm>

int main(int argc, char** argv) {

    std::vector<int> numbers;
    while (!std::cin.eof()) {
        int i;
        char ch;
        std::cin >> i;
        numbers.push_back(i);
        std::cin >> ch;
    }

    uint32_t max = *std::max_element(numbers.begin(), numbers.end());
    uint32_t min_found = max * numbers.size();
    for (int i = 0; i <= max; ++i) {
        int d = 0;
        for (int v : numbers) {
            if (i > v)
                d += i - v;
            else if (i < v)
                d += v - i;
        }
        if (d < min_found)
            min_found = d;
    }
    std::cout << min_found << std::endl;

    min_found = -1;
    for (int i = 0; i <= max; ++i) {
        int d = 0;
        for (int v : numbers) {
            if (i > v) {
                int diff = i - v;
                d += (diff * (diff+1)) / 2;
            } else if (i < v) {
                int diff = v - i;
                d += (diff * (diff+1)) / 2;
            }
        }
        if (d < min_found)
            min_found = d;
    }
    std::cout << min_found << std::endl;

    return 0;
}
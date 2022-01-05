#include <iostream>
#include <vector>

int main(int argc, char** argv) {

    std::vector<int> numbers;
    while (!std::cin.eof()) {
        int i;
        char ch;
        std::cin >> i;
        numbers.push_back(i);
        std::cin >> ch;
    }

    uint64_t old[7] = {0};
    uint64_t young[9] = {0};
    int o_idx = 0;
    int y_idx = 0;

    // initialise old.
    for (int v : numbers) {
        old[v]++;
    }

    uint64_t total = numbers.size();

    // part 1
    int day = 0;
    for (; day < 80; ++day) {
        total += old[o_idx];
        total += young[y_idx];
        old[o_idx] += young[y_idx];
        young[y_idx] = old[o_idx];
        // move idx to next place
        o_idx++;
        o_idx %= 7;
        y_idx++;
        y_idx %= 9;
    }
    std::cout << total << "\n";

    // part 2
    for (; day < 256; ++day) {
        total += old[o_idx];
        total += young[y_idx];
        old[o_idx] += young[y_idx];
        young[y_idx] = old[o_idx];
        // move idx to next place
        o_idx++;
        o_idx %= 7;
        y_idx++;
        y_idx %= 9;
    }
    std::cout << total << "\n";

    return 0;
}
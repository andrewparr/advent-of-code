#include <iostream>
#include <vector>

int main(int argc, char** argv) {

    std::vector<int> input;
    while (!std::cin.eof()) {
        int value;
        std::cin >> value;
        input.push_back(value);
    }

    int part_1 = 0;
    int part_2 = 0;
    for (size_t i = 1; i < input.size(); ++i) {
        if (input[i] > input[i-1])
            part_1++;
        if (i > 2 && input[i] > input[i-3])
            part_2++;
    }

    std::cout << part_1 << std::endl;
    std::cout << part_2 << std::endl;

    return 0;
}
#include <iostream>
#include <vector>
#include <utility>

int main(int argc, char** argv) {

    std::vector<std::pair<std::string, int>> input;
    while (!std::cin.eof()) {
        std::string s;
        int value;
        std::cin >> s >> value;
        input.emplace_back(s, value);
    }


    int forward = 0;
    int depth = 0;
    for (auto [s, v] : input) {
        if (s.compare("forward")==0)
            forward += v;
        else if (s.compare("down")==0)
            depth += v;
        else if (s.compare("up")==0)
            depth -= v;
    }
    std::cout << forward * depth << std::endl;

    forward = 0;
    depth = 0;
    int aim = 0;
    for (auto [s, v] : input) {
        if (s.compare("forward")==0) {
            forward += v;
            depth += (aim * v);
        } else if (s.compare("down")==0) {
            aim += v;
        } else if (s.compare("up")==0) {
            aim -= v;
        }
    }
    std::cout << forward * depth << std::endl;

    return 0;
}
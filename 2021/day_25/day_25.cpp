#include <iostream>
#include <set>
#include <utility>

int main(int argc, char** argv) {

    std::set<std::pair<int, int>> east;
    std::set<std::pair<int, int>> south;

    int y = 0;
    int x = 0;
    while (!std::cin.eof()) {
        std::string line;
        std::getline(std::cin, line);
        if (line.empty())
            continue;
        for (x = 0; x < line.length(); ++x) {
            if (line[x] == '>')
                east.emplace(x,y);
            else if (line[x] == 'v')
                south.emplace(x,y);
        }
        y++;
    }

    const int max_x = x;
    const int max_y = y;

    int step = 0;
    bool moved = true;
    while (moved) {
        step++;
        moved = false;
        std::set<std::pair<int, int>> new_east;
        for (auto& a : east) {
            x = (a.first + 1) % max_x;
            y = a.second;
            auto check = std::make_pair(x,y);
            if (east.count(check) == 0 && south.count(check) == 0) {
                new_east.emplace(check);
                moved = true;
            } else {
                new_east.emplace(a);
            }
        }
        east = std::move(new_east);
        std::set<std::pair<int, int>> new_south;
        for (auto& a : south) {
            x = a.first;
            y = (a.second + 1) % max_y;
            auto check = std::make_pair(x,y);
            if (east.count(check) == 0 && south.count(check) == 0) {
                new_south.emplace(check);
                moved = true;
            } else {
                new_south.emplace(a);
            }
        }
        south = std::move(new_south);
    };
    std::cout << step << std::endl;

    return 0;
}

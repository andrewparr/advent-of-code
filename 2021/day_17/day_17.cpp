#include <iostream>
#include <string>
#include <utility>
#include <regex>
#include <algorithm>

bool check_y(int vy, int& max_y, int a, int b) {
    int y = 0;
    int step = 0;
    bool hit_target = false;
    while (y > b) {
        y += vy;
        if (y > max_y)
            max_y = y;
        vy--;
        if (y <= a && y >= b)
            hit_target = true;
    }
    return hit_target;
}

bool check(int vx, int vy, std::pair<int,int> target_x, std::pair<int,int> target_y) {
    bool hit_target = false;
    int set = 0;
    int x = 0;
    int y = 0;
    while (y >= target_y.first) {
        x += vx;
        y += vy;
        if (vx > 0)
            vx--;
        else if (vx < 0)
            vx++;
        vy--;
        if (y <= target_y.second && y >= target_y.first
            && x <= target_x.second && x >= target_x.first)
            return true;
    }
    return false;
}

int main(int argc, char** argv) {

    std::string input;
    getline(std::cin, input);

    const std::regex re(R"(target area: x=(-?\d*)..(-?\d*), y=(-?\d*)..(-?\d*))");
    std::smatch match;
    if (regex_search(input, match, re)) {
        int x1 = stoi(match.str(1));
        int x2 = stoi(match.str(2));
        int y1 = stoi(match.str(3));
        int y2 = stoi(match.str(4));
        std::pair<int, int> target_x = std::make_pair(std::min(x1,x2), std::max(x1,x2));
        std::pair<int, int> target_y = std::make_pair(std::min(y1,y2), std::max(y1,y2));

        int global_max_y = 0;
        for (int vy = 0; vy < -(target_y.first-1); ++vy) {
            int max_y = 0;
            if (check_y(vy, max_y, target_y.second, target_y.first)) {
                if (max_y > global_max_y)
                    global_max_y = max_y;
            }

        }

        std::cout << global_max_y << std::endl;

        int count = 0;
        for (int vy = target_y.first; vy < -(target_y.first-1); ++vy) {
            for (int vx = 0; vx <= target_x.second; ++vx) {
                if (check(vx, vy, target_x, target_y)) {
                    count++;
                }
            }
        }
        std::cout << count << std::endl;
    }
    return 0;
}
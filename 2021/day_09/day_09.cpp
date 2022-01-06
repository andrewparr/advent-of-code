#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <list>
#include <algorithm>
#include <utility>

int main(int argc, char** argv) {

    std::vector<std::string> map;
    while (!std::cin.eof()) {
        std::string s;
        std::cin >> s;
        map.emplace_back(s);
    }

    std::vector<std::pair<int, int>> mins;

    int rows = map.size();
    int cols = map[0].size();

    bool checked[rows][cols];

    // part 1.
    int risk_level = 0;
    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            int value =  map[r][c] - '0';
            checked[r][c] = value == 9;
            bool minimal = true;
            if (r > 0) {
                int check =  map[r-1][c] - '0';
                if (check <= value)
                    minimal = false;
            }
            if (r < rows-1) {
                int check =  map[r+1][c] - '0';
                if (check <= value)
                    minimal = false;
            }
            if (c > 0) {
                int check =  map[r][c-1] - '0';
                if (check <= value)
                    minimal = false;
            }
            if (c < cols-1) {
                int check =  map[r][c+1] - '0';
                if (check <= value)
                    minimal = false;
            }
            if (minimal) {
                risk_level += value + 1;
                mins.push_back(std::make_pair(r,c));
            }
        }
    }
    std::cout << risk_level << std::endl;

    // part 2
    std::vector<int> basin_sizes;
    for (auto& a : mins) {
        int size = 0;
        // use list to push new items, repeat until list is empty
        std::list<std::pair<int, int>> queue;
        queue.push_back(a);
        while (!queue.empty()) {
            size++;
            int r = queue.front().first;
            int c = queue.front().second;
            checked[r][c] = true;
            queue.pop_front();
            if (r > 0) {
                if (!checked[r-1][c]) {
                    checked[r-1][c] = true;
                    queue.push_back(std::make_pair(r-1, c));
                }
            }
            if (r < rows-1) {
                if (!checked[r+1][c]) {
                    checked[r+1][c] = true;
                    queue.push_back(std::make_pair(r+1, c));
                }
            }
            if (c > 0) {
                if (!checked[r][c-1]) {
                    checked[r][c-1] = true;
                    queue.push_back(std::make_pair(r, c-1));
                }
            }
            if (c < cols-1) {
                if (!checked[r][c+1]) {
                    checked[r][c+1] = true;
                    queue.push_back(std::make_pair(r, c+1));
                }
            }
        }
        basin_sizes.push_back(size);
   }

    sort(basin_sizes.rbegin(), basin_sizes.rend());
    uint64_t part_2 = basin_sizes[0];
    part_2 *= basin_sizes[1];
    part_2 *= basin_sizes[2];
    std::cout << part_2 << std::endl;

    return 0;
}
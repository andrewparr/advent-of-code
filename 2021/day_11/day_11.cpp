#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <list>
#include <algorithm>
#include <utility>
#include <assert.h>
#include <cstring>

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

    int flash_count = 0;

    int part_1 = 0;
    int part_2 = 0;

    for (int step = 0;; ++step) {

        bool flashed[rows][cols];
        memset(flashed, 0, sizeof(flashed));
        std::list<std::pair<size_t, size_t>> flashes;

        for (size_t r = 0; r < rows; ++r) {
            for (size_t c = 0; c < cols; ++c) {
                map[r][c] += 1;
                int value =  map[r][c] - '0';
                if (value > 9) {
                    flashes.push_back(std::make_pair(r,c));
                    flashed[r][c] = true;
                }
            }
        }

        std::vector<std::pair<int,int>> neighbours = {{-1,-1},{0,-1},{1,-1},{-1,0},{1,0},{-1,1},{0,1},{1,1}};

        while (!flashes.empty()) {
            int r = flashes.front().first;
            int c = flashes.front().second;
            flashes.pop_front();
            // add one to all neighbours (if not flashed)
            // if these flash, add to the list.
            for (const auto& a : neighbours) {
                int nr = r + a.first;
                int nc = c + a.second;
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                    if (!flashed[nr][nc]) {
                        map[nr][nc] += 1;
                        int value =  map[nr][nc] - '0';
                        if (value > 9) {
                            flashes.push_back(std::make_pair(nr,nc));
                            flashed[nr][nc] = true;
                        }
                    }
                }
            }
        }

        int count = 0;
        for (size_t r = 0; r < rows; ++r) {
            for (size_t c = 0; c < cols; ++c) {
                if (flashed[r][c]) {
                    map[r][c] = '0';
                    count++;
                }
            }
        }
        flash_count += count;
        if (step == 99)
            part_1 = flash_count;

        if (count == rows * cols) {
            part_2 = step + 1;
            break;
        }
   }

    std::cout << part_1 << std::endl;
    std::cout << part_2 << std::endl;

    return 0;
}
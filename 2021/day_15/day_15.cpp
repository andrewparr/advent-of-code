#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <utility>
#include <cstring>

int main(int argc, char** argv) {

    std::vector<std::vector<int>> map;
    while (!std::cin.eof()) {
        std::string s;
        std::cin >> s;
        std::vector<int> row;
        for (char c : s) {
            row.emplace_back(c - '0');
        }
        map.emplace_back(row);
    }

    int rows = map.size();
    int cols = map[0].size();

    uint64_t risk[rows][cols];
    memset(risk, -1, sizeof(risk));
    risk[0][0] = map[0][0];

    std::queue<std::pair<int,int>> q;
    q.emplace(0,0);
    while (!q.empty()) {
        int x = q.front().first;
        int y = q.front().second;
        uint64_t cost = risk[y][x];
        q.pop();
        if (x > 0) {
            uint64_t new_risk = cost + map[y][x-1];
            if (new_risk < risk[y][x-1]) {
                risk[y][x-1] = new_risk;
                q.emplace(x-1, y);
            }
        }
        if (x < cols-1) {
            uint64_t new_risk = cost + map[y][x+1];
            if (new_risk < risk[y][x+1]) {
                risk[y][x+1] = new_risk;
                q.emplace(x+1, y);
            }
        }
        if (y > 0) {
            uint64_t new_risk = cost + map[y-1][x];
            if (new_risk < risk[y-1][x]) {
                risk[y-1][x] = new_risk;
                q.emplace(x, y-1);
            }
        }
        if (y < rows-1) {
            uint64_t new_risk = cost + map[y+1][x];
            if (new_risk < risk[y+1][x]) {
                risk[y+1][x] = new_risk;
                q.emplace(x, y+1);
            }
        }
    }
    std::cout << risk[rows-1][cols-1]  - risk[0][0] << std::endl;

    // resize map for part_2
    for (auto& a: map) {
        a.resize(cols*5);
        for (size_t i = cols; i < cols*5; ++i) {
            a[i] = ( a[i%cols] + (i/cols) ) % 9;
            if (a[i] == 0) a[i] = 9;
        }
    }
    map.resize(rows*5);
    for (size_t y = rows; y < rows*5; ++y) {
        map[y].resize(map[y%rows].size());
        for (size_t x = 0; x < map[y%rows].size(); ++x) {
            map[y][x] = ( map[y%rows][x] + (y/rows) ) % 9;
            if (map[y][x] == 0) map[y][x] = 9;
        }
    }

    rows = map.size();
    cols = map[0].size();

    uint64_t risk2[rows][cols];
    memset(risk2, -1, sizeof(risk2));
    risk2[0][0] = map[0][0];

    q = std::queue<std::pair<int,int>>();
    q.emplace(0,0);
    while (!q.empty()) {
        int x = q.front().first;
        int y = q.front().second;
        uint64_t cost = risk2[y][x];
        q.pop();
        if (x > 0) {
            uint64_t new_risk = cost + map[y][x-1];
            if (new_risk < risk2[y][x-1]) {
                risk2[y][x-1] = new_risk;
                q.emplace(x-1, y);
            }
        }
        if (x < cols-1) {
            uint64_t new_risk = cost + map[y][x+1];
            if (new_risk < risk2[y][x+1]) {
                risk2[y][x+1] = new_risk;
                q.emplace(x+1, y);
            }
        }
        if (y > 0) {
            uint64_t new_risk = cost + map[y-1][x];
            if (new_risk < risk2[y-1][x]) {
                risk2[y-1][x] = new_risk;
                q.emplace(x, y-1);
            }
        }
        if (y < rows-1) {
            uint64_t new_risk = cost + map[y+1][x];
            if (new_risk < risk2[y+1][x]) {
                risk2[y+1][x] = new_risk;
                q.emplace(x, y+1);
            }
        }
    }
    std::cout << risk2[rows-1][cols-1]  - risk2[0][0] << std::endl;

    return 0;
}
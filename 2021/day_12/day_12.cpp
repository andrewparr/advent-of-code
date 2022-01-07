#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <set>
#include <queue>
#include <list>
#include <algorithm>
#include <utility>
#include <assert.h>
#include <cstring>
#include <cctype>

bool valid1(const std::vector<std::string>& path) {
    std::map<std::string, int> map_count;
    for (auto str : path) {
        if (islower(str[0])) {
            map_count[str]++;
        }
    }
    int c = 0;
    for (auto a : map_count) {
        if (a.second > 1)
            c++;
    }
    // no small cave can be visited twice
    return c == 0;
}

bool valid2(const std::vector<std::string>& path) {
    std::map<std::string, int> map_count;
    for (auto str : path) {
        if (islower(str[0])) {
            map_count[str]++;
        }
    }
    int c = 0;
    std::string small_cave;
    for (auto a : map_count) {
        if (a.second > 1) {
            small_cave = a.first;
            c++;
        }
    }
    if (c == 0)
        return true;
    else if (c == 1 && map_count[small_cave] <= 2)
        return true;
    else
        return false;
}


int main(int argc, char** argv) {

    std::map<std::string, std::set<std::string>> edges;

    while (!std::cin.eof()) {
        std::string line;
        std::cin >> line;
        auto i = line.find('-');
        assert(i != std::string::npos);
        std::string node1 = line.substr(0, i);
        std::string node2 = line.substr(i+1);
        edges[node1].insert(node2);
        edges[node2].insert(node1);
    }

    int part_1 = 0;
    int part_2 = 0;
    std::queue<std::pair<std::string, std::vector<std::string>>> todo;
    std::vector<std::string> path;
    path.emplace_back("start");
    todo.emplace(make_pair("start", path));
    while (!todo.empty()) {
        auto top = std::move(todo.front());
        std::string node = top.first;
        std::vector<std::string> path = top.second;
        todo.pop();
        if (node == "end") {
            if (valid1(path))
                part_1++;
            if (valid2(path))
                part_2++;
        } else {
            auto next = edges[node];
            for (const auto& a : next) {
                if (a == "start")
                    continue;
                auto copy = path;
                copy.emplace_back(a);
                bool is_valid = valid1(path) || valid2(path);
                if (is_valid) {
                    todo.push(make_pair(a, copy));
                }
            }
        }
    }
    std::cout << part_1 << std::endl;
    std::cout << part_2 << std::endl;
    return 0;
}
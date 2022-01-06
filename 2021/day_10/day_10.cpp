#include <iostream>
#include <sstream>
#include <vector>
#include <array>
#include <map>
#include <list>
#include <stack>
#include <algorithm>
#include <utility>
#include <numeric>

int main(int argc, char** argv) {

    std::vector<std::string> lines;
    while (!std::cin.eof()) {
        std::string s;
        std::cin >> s;
        lines.emplace_back(s);
    }

    std::map<char, char> pair{{'(',')'},{'<','>'},{'{','}'},{'[',']'}};
    std::map<char, int> points{{')',3},{']',57},{'}',1197},{'>',25137}};

    std::vector<int> scores;
    std::vector<std::string> invalid_lines;
    std::vector<std::string> valid_lines;
    std::vector<std::string> completions;
    for (const auto& str : lines) {
        std::stack<char> list;
        bool valid = true;
        int score = 0;
        for (char c : str) {
            if (c == '(' || c == '[' || c == '{' || c == '<') {
                list.push(c);
            } else if (c == ')' || c == ']' || c == '}' || c == '>') {
                char expected = pair[list.top()];
                list.pop();
                if (c != expected) {
                    valid = false;
                    score += points[c];
                }
            }
        }
        if (!valid) {
            invalid_lines.push_back(str);
            scores.push_back(score);
        } else {
            valid_lines.push_back(str);
            std::stringstream ss;
            while (!list.empty()) {
                ss << pair[list.top()];
                list.pop();
            }
            completions.push_back(ss.str());
        }
    }
    std::cout << accumulate(scores.begin(), scores.end(), 0) << std::endl;

    std::map<char, int> complete_score_map{{')',1},{']',2},{'}',3},{'>',4}};
    std::vector<uint64_t> complete_scores;
    for (auto& str : completions) {
        uint64_t score = 0;
        for (char ch : str) {
            score *= 5;
            score += complete_score_map[ch];
        }
        complete_scores.push_back(score);
    }
    sort(complete_scores.begin(), complete_scores.end());
    std::cout << complete_scores[complete_scores.size()/2] << std::endl;

    return 0;
}
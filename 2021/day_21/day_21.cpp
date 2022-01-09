#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include <tuple>
#include <regex>

#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::uint128_t;

class Part2 {
public:
    Part2(int max_score);
    std::pair<uint128_t, uint128_t> solve(int p1, int p2, int s1, int s2);

private:
    int max_;

    std::vector<std::tuple<int, int, int>> throws;
    std::map<std::tuple<int, int, int, int>, std::pair<uint128_t, uint128_t>> cache_;
};

Part2::Part2(int max_score) : max_(max_score) {
    for (int d1 = 1; d1 <= 3; ++d1) {
        for (int d2 = 1; d2 <= 3; ++d2) {
            for (int d3 = 1; d3 <= 3; ++d3) {
                throws.emplace_back(d1,d2,d3);
            }
        }
    }
}

std::pair<uint128_t, uint128_t> Part2::solve(int p1, int p2, int s1, int s2) {
    if (s1 >= max_)
        return std::make_pair(1, 0);
    if (s2 >= max_)
        return std::make_pair(0, 1);

    auto q = std::make_tuple(p1, p2, s1, s2);
    auto a = cache_.find(q);
    if (a != cache_.end()) {
        return a->second;
    }
    auto score = std::make_pair<uint128_t,uint128_t>(0, 0);
    for (auto& a : throws) {
        auto [d1, d2, d3] = a;
        int new_p1 = p1 + d1 + d2 + d3;
        new_p1 %= 10;
        if (new_p1 == 0) new_p1 = 10;
        int new_s1 = s1 + new_p1;
        auto [p2_wins, p1_wins] = solve(p2, new_p1, s2, new_s1);
        score.first += p1_wins;
        score.second += p2_wins;
    }

    cache_[q] = score;
    return score;
}

int main(int argc, char** argv) {

    int player_1_start = 0;
    int player_2_start = 0;

    for (int i = 0; i < 2; ++i) {
        std::string input;
        getline(std::cin, input);
        const std::regex re(R"(Player \d starting position: (\d*))");
        std::smatch match;
        if (regex_search(input, match, re)) {
            if (i == 0)
                player_1_start = stoi(match.str(1));
            else
                player_2_start = stoi(match.str(1));
        }
    }

    int player_1 = 0;
    int player_2 = 0;

    int p1 = player_1_start;
    int p2 = player_2_start;

    int dice = 1;
    while (player_1 < 1000 && player_2 < 1000) {
        p1 += 3*dice + 3;
        dice += 3;
        p1 %= 10;
        if (p1 == 0) p1 = 10;
        player_1 += p1;
        if (player_1 >= 1000)
            break;
        p2 += 3*dice + 3;
        dice += 3;
        p2 %= 10;
        if (p2 == 0) p2 = 10;
        player_2 += p2;
    }

    int part_1 = std::min(player_1, player_2) * (dice-1);
    std::cout << part_1 << std::endl;

    Part2 part2(21);
    auto a = part2.solve(player_1_start, player_2_start, 0, 0);
    std::cout << std::max(a.first, a.second) << std::endl;

    return 0;
}
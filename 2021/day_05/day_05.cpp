#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>

int main(int argc, char** argv) {

    std::vector<std::pair<int,int>> from;
    std::vector<std::pair<int,int>> to;
    while (!std::cin.eof()) {
        int x,y;
        char ch;
        std::string s;
        std::cin >> x >> ch >> y;
        if (std::cin.eof())
            break;
        from.emplace_back(x,y);
        std::cin >> s;
        std::cin >> x >> ch >> y;
        to.emplace_back(x,y);
    }

    std::map<std::pair<int,int>, int> hv_lines;
    std::map<std::pair<int,int>, int> hvd_lines;
    for (size_t i = 0; i < from.size(); ++i) {
        auto a = from[i];
        auto b = to[i];
        if (a.first == b.first) {
            if (a.second < b.second) {
                for (int n = a.second; n <= b.second; ++n) {
                    auto idx = std::make_pair(a.first, n);
                    hv_lines[idx]++;
                    hvd_lines[idx]++;
                }
            } else {
                for (int n = b.second; n <= a.second; ++n) {
                    auto idx = std::make_pair(a.first, n);
                    hv_lines[idx]++;
                    hvd_lines[idx]++;
                }
            }
        } else if (a.second == b.second) {
            if (a.first < b.first) {
                for (int n = a.first; n <= b.first; ++n) {
                    auto idx = std::make_pair(n, a.second);
                    hv_lines[idx]++;
                    hvd_lines[idx]++;
                }
            } else {
                for (int n = b.first; n <= a.first; ++n) {
                    auto idx = std::make_pair(n, a.second);
                    hv_lines[idx]++;
                    hvd_lines[idx]++;
                }
            }
        } else {
            // assumed to be diagonal at 45 deg
            int x_inc = (a.first < b.first) ? 1 : -1;
            int y_inc = (a.second < b.second) ? 1 : -1;
            int x = a.first;
            int y = a.second;
            while (x != b.first) {
                auto idx = std::make_pair(x, y);
                hvd_lines[idx]++;
                x += x_inc;
                y += y_inc;
            }
            hvd_lines[std::make_pair(b.first, b.second)]++;
        }
    }

    int part_1 = std::count_if(hv_lines.begin(), hv_lines.end(), [](decltype(hv_lines)::value_type a) {
        return a.second > 1;
    });
    std::cout << part_1 << "\n";

    int part_2 = std::count_if(hvd_lines.begin(), hvd_lines.end(), [](decltype(hvd_lines)::value_type a) {
        return a.second > 1;
    });
    std::cout << part_2 << "\n";

    return 0;
}
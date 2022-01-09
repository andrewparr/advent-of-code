#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <set>
#include <queue>
#include <list>
#include <stack>
#include <algorithm>
#include <utility>
#include <assert.h>
#include <cstring>
#include <cctype>
#include <bitset>
#include <memory>
#include <regex>
#include <cmath>

class Image {
public:
    Image() {};
    friend std::istream& operator>>(std::istream& in, Image& img);
    friend std::ostream& operator<<(std::ostream& os, const Image& img);
    void enhance();
    int calc_idx(int x, int y);
    size_t count() {return points_.size();}

private:
    std::string map_;
    bool space_ = false;
    std::set<std::pair<int, int>> points_;
    int min_x = 0;
    int max_x = 0;
    int min_y = 0;
    int max_y = 0;
};


std::istream& operator>>(std::istream& in, Image& img) {
    getline(in, img.map_);
    assert(img.map_.length() == 512);

    std::string str;
    int y = 0;
    while (!in.eof()) {
        getline(in, str);
        if (!str.empty()) {
            int x = 0;
            for (x = 0; x < str.length(); ++x) {
                if (str[x] == '#') {
                    img.points_.insert(std::make_pair(x,y));
                }
            }
            img.max_x = std::max(img.max_x, x);
            y++;
        }
    }
    img.max_y = y;
    return in;
}

std::ostream& operator<<(std::ostream& os, const Image& img) {

    for (int y = img.min_y; y < img.max_y; ++y) {
        for (int x = img.min_x; x < img.max_x; ++x) {
            if (img.points_.count(std::make_pair(x,y)) > 0)
                os << "#";
            else
                os << ".";
        }
        os << "\n";
    }
    return os;
}

int Image::calc_idx(int xx, int yy) {
    int result = 0;
    for (int y = yy-1; y < yy+2; ++y) {
        for (int x = xx-1; x < xx+2; ++x) {
            result <<= 1;
            if (x < min_x || x >= max_x || y < min_y || y >= max_y) {
                if (space_)
                    result |= 0x01;
            } else  if (points_.count(std::make_pair(x,y)) > 0) {
                result |= 0x01;
            }
        }
    }
    return result;
}

void Image::enhance() {
    std::set<std::pair<int, int>> new_points;
    for (int x = min_x-2; x < max_x+2; ++x) {
        for (int y = min_y-2; y < max_y+2; ++y) {
            int idx = calc_idx(x, y);
            assert(idx >= 0 && idx <= 512);
            if (map_[idx] == '#')
                new_points.insert(std::make_pair(x,y));
        }
    }
    points_ = std::move(new_points);
    if (space_)
        space_ = map_[511] == '#';
    else
        space_ = map_[0] == '#';

    min_x -= 2;
    max_x += 2;
    min_y -= 2;
    max_y += 2;
}

int main(int argc, char** argv) {

    Image image;
    std::cin >> image;

    image.enhance();
    image.enhance();
    std::cout << image.count() << std::endl;

    for (int step = 2; step < 50; ++step) {
        image.enhance();
    }

    std::cout << image.count() << std::endl;

    return 0;
}
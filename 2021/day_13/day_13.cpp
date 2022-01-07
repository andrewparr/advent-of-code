#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

class paper {
public:
    paper(int x, int y);
    void set(int x, int y);
    void fold_x(int v);
    void fold_y(int v);
    int count();
    void print();
private:
    std::vector<std::string> data;
    int max_x;
    int max_y;
};

paper::paper(int x, int y) : max_x(x), max_y(y) {
    data.resize(y);
    for (auto& s : data) {
        s.resize(x);
        s.replace(s.begin(), s.end(), x, '.');
    }
}

void paper::set(int x, int y) {
    data[y][x] = '#';
}

void paper::print() {
    for (auto& s : data) {
        // this just makes is a bit easier to read.
        transform(s.begin(), s.end(), s.begin(), [](const char& ch) -> char{
            return ch == '.' ? ' ' : '#';
        });
        std::cout << s << std::endl;
    }
}

int paper::count() {
    int ret = 0;
    for (auto& s : data)
        for (char c : s)
            if (c == '#')
                ret++;
    return ret;
}

void paper::fold_x(int v) {
    for (auto& s : data) {
        for (size_t x = v+1, i = v-1; x < max_x; ++x, --i) {
            if (s[x]=='#')
                s[i] = '#';
        }
        s.resize(v);
    }
    max_x = v;
}

void paper::fold_y(int v) {
    for (size_t y = v+1, i = v-1; y < max_y; ++y, --i) {
        for (size_t x = 0; x < max_x; ++x) {
            if (data[y][x] == '#') {
                data[i][x] = '#';
            }
        }
    }
    data.resize(v);
    max_y = v;
}

int main(int argc, char** argv) {

    std::vector<std::pair<int, int>> dots;
    std::vector<std::pair<char, int>> folds;

    int max_x = 0;
    int max_y = 0;
    while (!std::cin.eof()) {
        std::string line;
        std::cin >> line;
        auto i = line.find(',');
        if (i != std::string::npos) {
            int x = stoi(line.substr(0, i));
            int y = stoi(line.substr(i+1));
            if (x > max_x) max_x = x;
            if (y > max_y) max_y = y;
            dots.emplace_back(x,y);
        } else {
            if (line.compare(0, 2, "x=") == 0) {
                int val = stoi(line.substr(2));
                folds.emplace_back('x', val);
            } else if (line.compare(0, 2, "y=") == 0) {
                int val = stoi(line.substr(2));
                folds.emplace_back('y', val);
            }
        }
    }

    paper p(max_x+1, max_y+1);
    for (auto& a : dots) {
        p.set(a.first, a.second);
    }
    int part_1 = 0;
    for (auto& a : folds) {
        if (a.first == 'x')
            p.fold_x(a.second);
        else
            p.fold_y(a.second);
        if (part_1 == 0)
            part_1 = p.count();
    }
    std::cout << part_1 << std::endl;
    p.print();
    return 0;
}

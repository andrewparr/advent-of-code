#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
// #include <map>
#include <set>
// #include <queue>
// #include <list>
// #include <stack>
#include <algorithm>
// #include <utility>
#include <assert.h>
// #include <cstring>
// #include <cctype>
// #include <bitset>
// #include <memory>
#include <regex>
// #include <cmath>

struct Point {
    Point() {}
    Point(long x, long y) : v{x,y} {}
    Point(long x, long y, long z) : v{x,y,z} {}
    Point(const Point& p) : v(p.v) {}
    std::vector<long> v;
    auto operator<=>(Point const& rhs) const = default;
    friend std::ostream& operator<<(std::ostream& os, const Point& p);
};

std::ostream& operator<<(std::ostream& os, const Point& p) {
    for (size_t i = 0; i < p.v.size(); ++i) {
        if (i > 0) os << ",";
        os << p.v[i];
    }
    return os;
}

Point operator-(const Point& lhs, const Point& rhs) {
    assert(lhs.v.size() == rhs.v.size());
    Point ret = lhs;
    for (size_t i = 0; i < ret.v.size(); ++i) {
        ret.v[i] -= rhs.v[i];
    }
    return ret;
}

Point operator+(const Point& lhs, const Point& rhs) {
    assert(lhs.v.size() == rhs.v.size());
    Point ret = lhs;
    for (size_t i = 0; i < ret.v.size(); ++i) {
        ret.v[i] += rhs.v[i];
    }
    return ret;
}

class Scanner : public std::set<Point> {
public:
    Scanner() {};
    friend std::istream& operator>>(std::istream& in, Scanner& scanner);
    friend std::ostream& operator<<(std::ostream& os, const Scanner& scanner);
    void add(const Point& p);
    bool permute();
    bool alignWith(Scanner& s, int num);
    bool isAligned() {return isAligned_;}
    Point alignment_;
protected:
    int number_{-1};
    std::set<Point> original_;
    bool isAligned_{false};

    int p_idx = 0;
    // Holds the different orientations of a cube. See http://www.euclideanspace.com/maths/discrete/groups/categorise/finite/cube/
    std::array<std::array<int,3>,24> p{{
        {1,2,3},
        // rotation around x
        {1,3,-2},
        {1,-2,-3},
        {1,-3,2},
        // rotation around y
        {3,2,-1},
        {-1,2,-3},
        {-3,2,1},
        // rotation around z
        {2,-1,3},
        {-1,-2,3},
        {-2,1,3},

        // rotation about opposite verticies
        {3,1,2},
        {2,3,1},
        {2,-3,-1},
        {-3,1,-2},
        {-2,-3,1},
        {3,-1,-2},
        {-3,-1,2},
        {-2,3,-1},

        // rotation about opposite lines
        {-2, -1,-3},
        {3,-2,1},
        {-1,3,2},
        {-1,-3,-2},
        {2,1,-3},
        {-3,-2,-1}
    }};
};

void Scanner::add(const Point& p) {
    original_.insert(p);
    insert(p);
}

std::ostream& operator<<(std::ostream& os, const std::array<int, 3>& a) {
    os << a[0] << "," << a[1] << "," << a[2];
    return os;
}

bool Scanner::permute() {

    p_idx = (p_idx + 1) % p.size();

    std::array<int, 3> p1 = p[p_idx];
    std::transform(p1.begin(), p1.end(), p1.begin(), [](int p) -> int {return abs(p)-1;});
    std::array<int, 3> p2 = p[p_idx];
    std::transform(p2.begin(), p2.end(), p2.begin(), [](int p) -> int {return p / abs(p);});

    clear();
    for (auto& a : original_) {
        emplace(a.v[p1[0]] * p2[0], a.v[p1[1]] * p2[1], a.v[p1[2]] * p2[2]);
    }
    return p_idx == 0;
}

bool Scanner::alignWith(Scanner& s, int num) {
    if (isAligned_)
        return true;
    bool found = false;
    Point diff;
    int max_matches = 0;
    for (int step = 0; step < 48; ++step) {
        for (auto& ref : s) {
            for (auto& test : *this) {
                diff = ref - test;
                int matches = 0;
                for (auto& a : *this) {
                    Point check = a + diff;
                    if (std::find(s.begin(), s.end(), check) != s.end())
                        matches++;
                }
                max_matches = std::max(max_matches, matches);
                if (matches >= num) {
                    found = true;
                    goto end;
                }
            }
        }
        permute();
    }
    end:
    if (found) {
        isAligned_ = true;
        int n = s.number_;
        if (n != 0 && s.isAligned_) {
            diff = diff + s.alignment_;
            n = 0;
        }
        alignment_ = diff;
    }
    return found;
}

std::istream& operator>>(std::istream& in, Scanner& scanner) {
    std::string str;
    getline(in, str);

    const std::regex title("--- scanner (\\d*) ---");
    std::smatch match;
    if (!std::regex_search(str, match, title)) {
        in.setstate(std::ios_base::failbit);
        return in;
    }
    scanner.number_ = std::stoi(match.str(1));

    const std::regex reg("(-?\\d*),(-?\\d*),(-?\\d*)");
    while (!in.eof()) {
        getline(in, str);
        if (!regex_search(str, match, reg))
            break;
        scanner.add(Point(stoi(match.str(1)), stoi(match.str(2)), stoi(match.str(3))));
    }
    return in;
}

std::ostream& operator<<(std::ostream& os, const Scanner& scanner) {
    os << "--- scanner " << scanner.number_ << " ---\n";
    for (auto& p : scanner) {
        os << p << "\n";
    }
    os << (scanner.isAligned_?"is aligned":"not aligned") << "\n";
    return os;
}

int main(int argc, char** argv) {

    std::vector<Scanner> scanners;
    while (!std::cin.eof() && !std::cin.fail()) {
        Scanner scanner;
        std::cin >> scanner;
        scanners.push_back(scanner);
    }

    std::cout << "here\n";

    std::set<int> to_align;
    for (int i = 1; i < scanners.size(); ++i) {
        to_align.insert(i);
    }

    std::set<int> check;
    check.insert(0);
    while (!to_align.empty()) {
        std::set<int> new_check;
        for (int i : to_align) {
            for (int j : check) {
                if (scanners[i].alignWith(scanners[j], 12)) {
                    new_check.insert(i);
                }
            }
        }
        for (int x : new_check) {
            to_align.erase(x);
        }
        check = std::move(new_check);
    }

    // Now add all the beacons relative to scanner 1.
    std::set<Point> beacons;
    for (auto& p : scanners[0])
        beacons.insert(p);
    for (size_t i = 1; i < scanners.size(); ++i) {
        for (auto p : scanners[i]) {
            p = p + scanners[i].alignment_;
            beacons.insert(p);
        }
    }
    std::cout << beacons.size() << std::endl;

    scanners[0].alignment_ = Point(0,0,0);

    uint64_t max_manhattan = 0;
    for (size_t i = 0; i < scanners.size(); ++i) {
        for (size_t j = i+1; j < scanners.size(); ++j) {
            uint64_t mhd = abs(scanners[i].alignment_.v[0] - scanners[j].alignment_.v[0])
                + abs(scanners[i].alignment_.v[1] - scanners[j].alignment_.v[1])
                + abs(scanners[i].alignment_.v[2] - scanners[j].alignment_.v[2]);
            if (mhd > max_manhattan)
                 max_manhattan = mhd;
        }
    }
    std::cout << max_manhattan << std::endl;

    return 0;
}
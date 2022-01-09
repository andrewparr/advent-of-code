#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <optional>

#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::int128_t;

struct Cube {
    Cube(int128_t x1, int128_t x2, int128_t y1, int128_t y2, int128_t z1, int128_t z2) :
        x1(x1), x2(x2), y1(y1), y2(y2), z1(z1), z2(z2) {
        assert(x2 >= x1);
        assert(y2 >= y1);
        assert(z2 >= z1);
    }

    auto operator<=>(Cube const& rhs) const = default;

    bool exceedsLimit(int128_t limit) const {
        if (abs(x1) > limit || abs(x2) > limit || abs(y1) > limit || abs(y2) > limit || abs(z1) > limit || abs(z2) > limit)
            return true;
        return false;
    }

    int128_t Size() const {
        return (1+x2-x1) * (1+y2-y1) * (1+z2-z1);
    }

    std::optional<Cube> Intersects(const Cube& cube) const;

    int128_t x1;
    int128_t x2;
    int128_t y1;
    int128_t y2;
    int128_t z1;
    int128_t z2;
};

std::ostream& operator<<(std::ostream& os, const Cube& cube) {
    os << "(" << cube.x1 << " " << cube.x2 << " " << cube.y1 << " " << cube.y2 << " " << cube.z1 << " " << cube.z2 << ")\n";
    return os;
}

std::optional<Cube> Cube::Intersects(const Cube& cube) const {
    std::optional<Cube> result;
    int128_t xx1 = std::max(x1, cube.x1);
    int128_t xx2 = std::min(x2, cube.x2);
    int128_t yy1 = std::max(y1, cube.y1);
    int128_t yy2 = std::min(y2, cube.y2);
    int128_t zz1 = std::max(z1, cube.z1);
    int128_t zz2 = std::min(z2, cube.z2);
    if (xx2 >= xx1 && yy2 >= yy1 && zz2 >= zz1) {
        result = Cube(xx1, xx2, yy1, yy2, zz1, zz2);
    }
    return result;
}

class Reactor {
public:
    Reactor() {};
    Reactor(const std::optional<int128_t>& limit) : limit_(limit) {};

    void on(const Cube& cube);
    void off(const Cube& cube);

    int128_t count();

private:
    std::optional<int128_t> limit_;
    std::vector<Cube> state_;
};

int128_t Reactor::count() {
    int128_t total = 0;
    for (auto cube : state_) {
        total += cube.Size();
    }
    return total;
}

void Reactor::on(const Cube& cube) {
    if (limit_.has_value() && cube.exceedsLimit(limit_.value()))
        return;
    off(cube);
    state_.emplace_back(cube);
}

void Reactor::off(const Cube& cube) {
    if (limit_.has_value() && cube.exceedsLimit(limit_.value()))
        return;

    std::vector<Cube> new_state;
    for (auto& a : state_) {
        auto intersect = cube.Intersects(a);
        if (intersect.has_value()) {
            {
                int128_t x1 = a.x1;
                int128_t x2 = intersect.value().x1 - 1;
                if (x1 <= x2)
                    new_state.emplace_back(x1, x2, a.y1, a.y2, a.z1, a.z2);
            }
            {
                int128_t x1 = intersect.value().x2 + 1;
                int128_t x2 = a.x2;
                if (x1 <= x2)
                    new_state.emplace_back(x1, x2, a.y1, a.y2, a.z1, a.z2);
            }

            {
                int128_t y1 = a.y1;
                int128_t y2 = intersect.value().y1 - 1;
                if (y1 <= y2)
                    new_state.emplace_back(intersect.value().x1, intersect.value().x2, y1, y2, a.z1, a.z2);
            }
            {
                int128_t y1 = intersect.value().y2 + 1;
                int128_t y2 = a.y2;
                if (y1 <= y2)
                    new_state.emplace_back(intersect.value().x1, intersect.value().x2, y1, y2, a.z1, a.z2);
            }

            {
                int128_t z1 = a.z1;
                int128_t z2 = intersect.value().z1 - 1;
                if (z1 <= z2)
                    new_state.emplace_back(intersect.value().x1, intersect.value().x2, intersect.value().y1, intersect.value().y2, z1, z2);
            }
            {
                int128_t z1 = intersect.value().z2 + 1;
                int128_t z2 = a.z2;
                if (z1 <= z2)
                    new_state.emplace_back(intersect.value().x1, intersect.value().x2, intersect.value().y1, intersect.value().y2, z1, z2);
            }

        } else {
            new_state.emplace_back(a);
        }
    }

    state_ = std::move(new_state);
}

int main(int argc, char** argv) {

    std::vector<std::string> lines;
    while (!std::cin.eof() && !std::cin.fail()) {
        std::string line;
        getline(std::cin, line);
        lines.push_back(line);
    }

    Reactor part_1(50);
    Reactor part_2;

    for (auto& line : lines) {
        const std::regex reg("(on|off) x=(-?\\d*)..(-?\\d*),y=(-?\\d*)..(-?\\d*),z=(-?\\d*)..(-?\\d*)");
        std::smatch match;
        if (regex_search(line, match, reg)) {
            bool on = match.str(1) == "on";
            int128_t x1 = stoi(match.str(2));
            int128_t x2 = stoi(match.str(3));
            int128_t y1 = stoi(match.str(4));
            int128_t y2 = stoi(match.str(5));
            int128_t z1 = stoi(match.str(6));
            int128_t z2 = stoi(match.str(7));
            Cube cube(x1, x2, y1, y2, z1, z2);
            if (on) {
                part_1.on(cube);
                part_2.on(cube);
            } else {
                part_1.off(cube);
                part_2.off(cube);
            }
        }
    }

    std::cout << part_1.count() << std::endl;
    std::cout << part_2.count() << std::endl;

    return 0;
}
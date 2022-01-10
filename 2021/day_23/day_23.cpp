#include <iostream>
#include <array>
#include <algorithm>
#include <vector>
#include <queue>
#include <map>
#include <functional>
#include <cstdlib>
#include <set>

class Burrow {
public:
    struct Room {
        size_t idx;
        char home;
        int cost;
        std::string contents;
    };
    struct State {
        std::string hallway;
        std::map<char, Room> rooms;
    };

    Burrow() = default;
    void addHallway(const std::string& hallway);
    void addRoom(size_t idx, char home, int cost, const std::string& contents);
    int solve();
    bool isSolved(const State& s);
    typedef std::function<void(const State&, int)> callback;
    void nextStates(const State& state, callback fn);

private:
    std::set<size_t> rm_idxs_;
    std::map<char, size_t> rm_;
    State initial_state_;
};

std::ostream& operator<<(std::ostream& os, const Burrow::State& s) {
    os << s.hallway << std::endl;
    for (auto& r : s.rooms) {
        os << r.second.contents << std::endl;
    }
    return os;
}

void Burrow::addHallway(const std::string& hallway) {
    initial_state_.hallway = hallway;
}

void Burrow::addRoom(size_t idx, char home, int cost, const std::string& contents) {
    Room rm;
    rm.idx = idx;
    rm.home = home;
    rm.cost = cost;
    rm.contents = contents;
    initial_state_.rooms[home] = rm;
    rm_[home] = idx;
    rm_idxs_.insert(idx);
}

bool Burrow::isSolved(const State& s) {
    for (auto c : s.hallway) {
        if (c != '.')
            return false;
    }
    for (auto r : s.rooms) {
        for (auto c : r.second.contents) {
            if (c != r.first)
                return false;
        }
    }
    return true;
}

void Burrow::nextStates(const State& state, callback fn) {
    // check if any amphipods in hallway can go to their room.
    for (size_t i = 0; i < state.hallway.length(); ++i) {
        char c = state.hallway[i];
        bool ok = true;
        if (c != '.') {
            size_t dst = rm_[c];
            int inc = dst > i ? +1 : -1;
            // check clean path to room.
            for (size_t n = i + inc; n != dst; n += inc) {
                if (state.hallway[n] != '.') {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;
            // now check room only has amphipods of correct type.
            for (char ch : state.rooms.at(c).contents) {
                if (ch != c && ch != '.') {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;
            // If we're here, amphipod can move to their home.
            int cost = abs(dst-i) * state.rooms.at(c).cost;
            size_t rmi = 0;
            // go into room as far as possible.
            while (state.rooms.at(c).contents[rmi] == '.')
                rmi++;
            cost += rmi * state.rooms.at(c).cost;
            State ns = state;
            ns.hallway[i] = '.';
            ns.rooms.at(c).contents[rmi-1] = c;
            fn(ns, cost);
        }
    }
    // For all the rooms, check where the top Amphipod can move to.
    for (auto rm : state.rooms) {
        // Skip the room unless it contains a forign Amphipod.
        bool skip = true;
        for (char ch : rm.second.contents) {
            if (ch != rm.first && ch != '.') {
                skip = false;
                break;
            }
        }
        if (skip) continue;
        size_t rmi = 0;
        bool ok = false;
        for (;rmi < rm.second.contents.length(); ++rmi) {
            if (rm.second.contents[rmi] != '.') {
                ok = true;
                break;
            }
        }
        if (!ok) continue;   // room was empty
        // going left of room.
        for (size_t dst = rm.second.idx; dst < state.hallway.length() && state.hallway[dst] == '.'; --dst) {
            // can't stop outside of a room.
            if (rm_idxs_.count(dst) > 0)
                continue;
            State ns = state;
            char ch = rm.second.contents[rmi];
            ns.hallway[dst] = ch;
            ns.rooms.at(rm.first).contents[rmi] = '.';
            int cost = (1 + rmi + (rm.second.idx - dst)) * state.rooms.at(ch).cost;
            fn(ns, cost);
        }
        // now check going right
        for (size_t dst = rm.second.idx; dst < state.hallway.length() && state.hallway[dst] == '.'; ++dst) {
            // can't stop outside of a room.
            if (rm_idxs_.count(dst) > 0)
                continue;
            State ns = state;
            char ch = rm.second.contents[rmi];
            ns.hallway[dst] = ch;
            ns.rooms.at(rm.first).contents[rmi] = '.';
            int cost = (1 + rmi + (dst - rm.second.idx)) * state.rooms.at(ch).cost;
            fn(ns, cost);
        }
    }

}

bool operator<(const Burrow::State& lhs, const Burrow::State& rhs) {
    if (lhs.hallway != rhs.hallway)
        return lhs.hallway < rhs.hallway;
    auto lhs_iter = lhs.rooms.begin();
    auto rhs_iter = rhs.rooms.begin();
    for (; lhs_iter != lhs.rooms.end(); ++lhs_iter, ++rhs_iter) {
        if (lhs_iter->second.contents != rhs_iter->second.contents)
            return lhs_iter->second.contents < rhs_iter->second.contents;
    }
    return false;
};


int Burrow::solve() {

    typedef std::pair<int, State> costState;
    auto comp = [](costState& lhs, costState& rhs) -> bool {
        return lhs.first > rhs.first;
    };

    std::priority_queue<costState, std::vector<costState>, decltype(comp)> queue(comp);
    std::map<State, int> visited;

    queue.emplace(0, initial_state_);
    visited[initial_state_] = 0;

    while (!queue.empty()) {
        costState cs = queue.top();
        queue.pop();
        if (isSolved(cs.second))
            return cs.first;
        nextStates(cs.second, [&](State new_state, int cost) {
            if (visited.count(new_state) == 0) {
                queue.emplace(cost + cs.first, new_state);
                visited[new_state] = cost + cs.first;
            } else {
                if (visited[new_state] > cost + cs.first) {
                    queue.emplace(cost + cs.first, new_state);
                    visited[new_state] = cost + cs.first;
                }
            }
        });
    }

    return 0;
}

int main(int argc, char** argv) {

    Burrow burrow;

    // my puzzle input
    // #############
    // #...........#
    // ###D#B#A#C###
    //   #B#D#A#C#
    //   #########
    // TODO: add logic to parse input.txt

    burrow.addHallway("...........");
    burrow.addRoom(2, 'A', 1, "DB");
    burrow.addRoom(4, 'B', 10, "BD");
    burrow.addRoom(6, 'C', 100, "AA");
    burrow.addRoom(8, 'D', 1000, "CC");
    std::cout << burrow.solve() << "\n";

    // part 2
    burrow.addHallway("...........");
    burrow.addRoom(2, 'A', 1, "DDDB");
    burrow.addRoom(4, 'B', 10, "BCBD");
    burrow.addRoom(6, 'C', 100, "ABAA");
    burrow.addRoom(8, 'D', 1000, "CACC");

    std::cout << burrow.solve() << "\n";
    return 0;
}
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <utility>
#include <regex>
#include <string>
#include <string_view>
#include <optional>
#include <ranges>
#include <assert.h>

enum class Op {inp, add, mul, div, mod, eql};

std::ostream& operator<<(std::ostream& os, Op o) {
    switch (o) {
        case Op::inp: os << "inp"; break;
        case Op::add: os << "add"; break;
        case Op::mul: os << "mul"; break;
        case Op::div: os << "div"; break;
        case Op::mod: os << "mod"; break;
        case Op::eql: os << "eql"; break;
        default:
            assert(false);
    }
    return os;
}

Op strToOp(std::string_view str) {
    if (str.compare("inp") == 0)
        return Op::inp;
    else if (str.compare("add") == 0)
        return Op::add;
    else if (str.compare("mul") == 0)
        return Op::mul;
    else if (str.compare("div") == 0)
        return Op::div;
    else if (str.compare("mod") == 0)
        return Op::mod;
    else if (str.compare("eql") == 0)
        return Op::eql;
    else
        assert(false);
}

std::istream& operator>>(std::istream& is, Op o) {
    std::string str;
    is >> str;
    o = strToOp(str);
    return is;
}

class ALU {
public:
    ALU() {}
    void add(Op o, char a, char b) {
        program_.emplace_back(o, a, b);
    }
    void add(Op o, char a, int val) {
        program_.emplace_back(o, a, val);
    }
    std::optional<int64_t> run(int ip, int64_t z, bool max);
    void clear() {
        cache_.clear();
    }

private:
    struct Instruction {
        explicit Instruction(Op o, char a, char b) : o(o), a(a-'w'), b_or_val(b-'w'), b_is_val(false) {}
        explicit Instruction(Op o, char a, int b) : o(o), a(a-'w'), b_or_val(b), b_is_val(true) {}
        Op o;
        int64_t a;
        int64_t b_or_val;
        bool b_is_val;
    };

    std::vector<Instruction> program_;
    std::map<std::pair<int, int64_t>, std::optional<int64_t>> cache_;
};


std::ostream& operator<<(std::ostream& os, const std::array<int64_t,4>& wxyz) {
    os << wxyz[0] << "," << wxyz[1] << "," << wxyz[2] << "," << wxyz[3];
    return os;
}

std::optional<int64_t> ALU::run(int _ip, int64_t z, bool max) {
    auto a = cache_.find(std::make_pair(_ip, z));
    if (a != cache_.end()) {
        return a->second;
    }

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    if (max)
        reverse(numbers.begin(), numbers.end());
    auto v = std::views::all(numbers);
    for (int64_t inp : v) {
        int ip = _ip;
        std::array<int64_t,4> wxyz{0,0,0,z};

        Instruction i = program_[ip];
        // Each part run starts with an inp instruction
        assert(i.o == Op::inp);
        wxyz[i.a] = inp;
        ip += 1;
        // run until we get to the end or another inp instruction.
        while (ip < program_.size()) {
            i = program_[ip];
            switch (i.o) {
                case Op::inp:
                    {
                        auto a = run(ip, wxyz[3], max);
                        if (a.has_value()) {
                            int64_t full_inp = (a.value() * 10) + inp;
                            // This will build the number up reversed.
                            // I could use log10 to find how many digits it has to keep this in the correct order!
                            // but it doesn't seem worth it.
                            cache_[std::make_pair(ip, wxyz[3])] = full_inp;
                            return full_inp;
                        }
                    }
                    break;
                case Op::add:
                    wxyz[i.a] += i.b_is_val ? i.b_or_val : wxyz[i.b_or_val];
                    break;
                case Op::mul:
                    wxyz[i.a] *= i.b_is_val ? i.b_or_val : wxyz[i.b_or_val];
                    break;
                case Op::div:
                    wxyz[i.a] /= i.b_is_val ? i.b_or_val : wxyz[i.b_or_val];
                    break;
                case Op::mod:
                    wxyz[i.a] %= i.b_is_val ? i.b_or_val : wxyz[i.b_or_val];
                    break;
                case Op::eql:
                    wxyz[i.a] = i.b_is_val ? wxyz[i.a] == i.b_or_val : wxyz[i.a] == wxyz[i.b_or_val];
                    break;
                default:
                    assert(false);
            }
            ip += 1;
        }

        if (wxyz[3] == 0) {
            cache_[std::make_pair(ip, 0)] = inp;
            return inp;
        }
    }
    cache_[std::make_pair(_ip, z)] = std::optional<int64_t>();
    return std::optional<int64_t>();
}

int main(int argc, char** argv) {
    ALU alu;

    while (!std::cin.eof()) {
        std::string line;
        getline(std::cin, line);
        const std::regex imp_re("inp ([w-z])");
        const std::regex op_re("(add|mul|div|mod|eql) ([w-z]) ([w-z]|-?\\d*)");
        std::smatch match;
        if (regex_search(line, match, imp_re)) {
            alu.add(Op::inp, match.str(1)[0], 0);
        } else  if (regex_search(line, match, op_re)) {
            if (match.str(3)[0] >= 'w' || match.str(3)[0] >= 'z')
                alu.add(strToOp(match.str(1).c_str()), match.str(2)[0], match.str(3)[0]);
            else
                alu.add(strToOp(match.str(1).c_str()), match.str(2)[0], stoi(match.str(3)));
        } else {
            std::cerr << "parse error: " << line << "\n";
        }
    }

    auto ret = alu.run(0, 0, true);
    assert(ret.has_value());
    std::stringstream ss;
    ss << ret.value();
    std::string str = ss.str();
    reverse(str.begin(), str.end());
    std::cout << str << std::endl;

    alu.clear();
    ss.str("");
    ret = alu.run(0, 0, false);
    assert(ret.has_value());
    ss << ret.value();
    str = ss.str();
    reverse(str.begin(), str.end());
    std::cout << str << std::endl;

    return 0;
}

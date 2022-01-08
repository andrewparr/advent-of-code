#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <utility>
#include <assert.h>
#include <memory>

class node {
public:
    node(int64_t v) : is_literal(true), value(v) {}

    node(std::shared_ptr<node> l, std::shared_ptr<node> r)
        : is_literal(false), left(l), right(r) {}

    bool is_literal;
    int64_t value;
    std::shared_ptr<node> left;
    std::shared_ptr<node> right;
};

std::shared_ptr<node> parse(std::string str, int d = 0) {
    if (str[0] == '[') {
        size_t mid, end;
        size_t c = 0;
        for (size_t i = 1; ; ++i) {
            assert(str[i] != '\0');
            if (str[i] == '[') {
                c++;
            }
            if (str[i] == ']') {
                if  (c == 0) {
                     end = i;
                     break;
                }
                c--;
            }
            if (str[i] == ',' && c == 0) {
                mid = i;
            }
        }
        auto left = parse(str.substr(1, mid-1), d+1);
        auto right = parse(str.substr(mid+1, end-1), d+1);
        return std::make_shared<node>(left, right);
    } else {
        std::istringstream ss(str);
        int64_t v;
        ss >> v;
        return  std::make_shared<node>(v);
    }
}

std::ostream& operator<<(std::ostream& out, const node& n) {
    if (n.is_literal)
        out << n.value;
    else
        out << "[" << *n.left << "," << *n.right << "]";
    return out;
}

bool explode(std::shared_ptr<node> n) {
    std::shared_ptr<node> left;
    int64_t right;
    bool exploded = false;

    std::stack<std::pair<int, std::shared_ptr<node>>> store;
    std::shared_ptr<node> current(n);
    int depth = 0;
    while (true) {
        if (exploded) {
            if (current->is_literal) {
                current->value += right;
                break;
            } else {
                store.emplace(depth + 1, current->right);
                current = current->left;
                depth++;
            }
        } else {
            if (depth >= 4 && !current->is_literal) {
                assert(current->left->is_literal);
                assert(current->right->is_literal);
                if (left != nullptr) {
                    left->value += current->left->value;
                }
                right = current->right->value;
                current->left.reset();
                current->right.reset();
                current->is_literal = true;
                current->value = 0;
                if (store.empty())
                    break;
                current = store.top().second;
                depth = store.top().first;
                store.pop();
                exploded = true;
                continue;
            }
            if (current->is_literal) {
                left = current;
                if (store.empty())
                    break;
                current = store.top().second;
                depth = store.top().first;
                store.pop();
                continue;
            } else {
                store.emplace(depth + 1, current->right);
                current = current->left;
                depth++;
            }
       }
    };
    return exploded;
}

bool split(std::shared_ptr<node> n) {
    bool split = false;
    std::stack<std::shared_ptr<node>> store;
    std::shared_ptr<node> current(n);
    while (true) {
        if (current->is_literal) {
            if (current->value >= 10) {
                current->is_literal = false;
                current->left = std::make_shared<node>(current->value / 2);
                current->right = std::make_shared<node>(++current->value / 2);
                split = true;
                break;
            }
            if (store.empty())
                break;
            current = store.top();
            store.pop();
            continue;
        } else {
            store.emplace(current->right);
            current = current->left;
        }
    };
    return split;
}

std::shared_ptr<node> add(std::shared_ptr<node> left, std::shared_ptr<node> right) {
    auto n = std::make_shared<node>(left, right);
    while (true) {
        if (explode(n) || split(n))
            continue;
        break;
    }
    return n;
}

int64_t magnitude(std::shared_ptr<node> n) {
    if (n->is_literal)
        return n->value;

    return (3 * magnitude(n->left)) + (2 * magnitude(n->right));
}

int main(int argc, char** argv) {

    std::vector<std::string> numbers;
    while (!std::cin.eof()) {
        std::string str;
        std::cin >> str;
        if (!str.empty())
           numbers.emplace_back(str);
    }

    auto n = parse(numbers[0]);
    for (size_t i = 1; i < numbers.size(); ++i) {
        n = add(n, parse(numbers[i]));
    }
    std::cout << magnitude(n) << std::endl;

    int64_t max_mag = 0;
    for (size_t i = 0; i < numbers.size(); ++i) {
        for (size_t j = 0; j < numbers.size(); ++j) {
            if (i == j)
                continue;
            auto n = parse(numbers[i]);
            auto m = parse(numbers[j]);
            int64_t mag = magnitude(add(n, m));
            if (mag > max_mag)
                max_mag = mag;
        }
    }
    std::cout << max_mag << std::endl;

    return 0;
}
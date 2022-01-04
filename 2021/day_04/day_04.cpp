#include <iostream>
#include <vector>

class BingoCard {
public:
    BingoCard();
    friend std::istream& operator>>(std::istream& in, BingoCard& t);
    friend std::ostream& operator<<(std::ostream& os, BingoCard& t);
    bool isValid();
    bool play(int n);
    int sum();
    bool has_won() {return won_;}
private:
    std::vector<int> grid_;
    std::vector<bool> check_;
    bool won_;
};

BingoCard::BingoCard() {
    check_.resize(25, false);
}

bool BingoCard::isValid() {
    // a grid is a 5x5 grid of integers.
    return grid_.size() == 25;
}

bool BingoCard::play(int n) {
    for (size_t i = 0; i < 25; ++i) {
        if (grid_[i] == n) {
            check_[i] = true;
            break;
        }
    }
    won_ |= check_[0] && check_[1] && check_[2] && check_[3] && check_[4];
    won_ |= check_[5] && check_[6] && check_[7] && check_[8] && check_[9];
    won_ |= check_[10] && check_[11] && check_[12] && check_[13] && check_[14];
    won_ |= check_[15] && check_[16] && check_[17] && check_[18] && check_[19];
    won_ |= check_[20] && check_[21] && check_[22] && check_[23] && check_[24];

    won_ |= check_[0] && check_[5] && check_[10] && check_[15] && check_[20];
    won_ |= check_[1] && check_[6] && check_[11] && check_[16] && check_[21];
    won_ |= check_[2] && check_[7] && check_[12] && check_[17] && check_[22];
    won_ |= check_[3] && check_[8] && check_[13] && check_[18] && check_[23];
    won_ |= check_[4] && check_[9] && check_[14] && check_[19] && check_[24];
    return won_;
}

int BingoCard::sum() {
    int sum = 0;
    for (size_t i = 0; i < 25; ++i) {
        if (!check_[i])
            sum += grid_[i];
    }
    return sum;
}

std::istream& operator>>(std::istream& in, BingoCard& t) {
    // A grid consists of 25 integers
    int v;
    std::cin >> v;
    t.grid_.push_back(v);
    while (!std::cin.eof() && t.grid_.size() < 25) {
        std::cin >> v;
        t.grid_.push_back(v);
    }
    return in;
};

std::ostream& operator<<(std::ostream& os, BingoCard& t) {
    os << "grid: ";
    for (int v : t.grid_)
        os << v << " ";
    os << "\n";
    os << "check: ";
    for (bool b : t.check_)
        os << b << " ";
    os << "\n";
    return os;
}

int main(int argc, char** argv) {

    // first read in the numbers
    std::vector<int> numbers;
    int i;
    char ch;
    std::cin >> i;
    numbers.push_back(i);
    std::cin >> ch;
    while (ch == ',' ) {
        std::cin >> i;
        numbers.push_back(i);
        std::cin >> ch;
    }

    std::vector<BingoCard> cards;
    while (!std::cin.eof()) {
        BingoCard bc;
        std::cin >> bc;
        if (bc.isValid())
            cards.emplace_back(bc);
    }

    int total_won = 0;
    for (int n : numbers) {
        for (BingoCard& c : cards) {
            if (!c.has_won() && c.play(n)) {
                total_won++;
                if (total_won == 1) {
                    std::cout << (c.sum() * n) << "\n";
                }
                if (total_won == cards.size()) {
                    std::cout << (c.sum() * n) << "\n";
                    break;
                }
            }
        }
    }

    return 0;
}
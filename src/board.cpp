#include "board.h"
#include "plot.cpp"
#include "functions.cpp"

#include <vector>
#include <string>
#include <random>

using namespace std;

vector<unsigned char> Board::rollDice() {
    random_device random;
    mt19937 generate(random());
    uniform_int_distribution<> distr(1, 6);
    unsigned char die1 = distr(generate);
    unsigned char die2 = distr(generate);
    vector vect{die1, die2};
    return vect;
}

void Board::createPlots() {
    this->plots.push_back(Plot(
        {"GO SQUARE"},
        {
            {"NAME", "GO ------>"},
            {"COLORCODE", functions::ANSI_WHITE}
        },
        {}
    ));

}

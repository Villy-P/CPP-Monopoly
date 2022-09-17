#ifndef BOARD_H
#define BOARD_H

#include "plot.hpp"

#include <vector>
#include <string>

class Player;

class Board {
    public:
        std::vector<Player> players;
        std::vector<Plot> plots;

        std::vector<unsigned char> rollDice();
        std::string getStringProperty(int index, std::string propertyName);
        Plot getPlot(int index);
        void createPlots();
};

#endif

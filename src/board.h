#ifndef BOARD_H
#define BOARD_H

#include "player.h"
#include "plot.h"

#include <vector>
#include <string>

class Board {
    public:
        std::vector<Player> players;
        std::vector<Plot> plots;

        std::vector<unsigned char> rollDice();
        std::string getStringProperty(unsigned char index, std::string propertyName);
        void createPlots();
};

#endif

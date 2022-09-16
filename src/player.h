#ifndef PLAYER_H
#define PLAYER_H

#include "plot.h"

#include <string>
#include <vector>

class Player {
    public:
        bool isMainPlayer;
        std::string name;
        std::string identifier;
        int cash = 1500;
        int plotPosition = 0;
        std::vector<Plot> ownedPlots;

        Player();

        void movePlayer();
        bool reduceMoney();
};

#endif

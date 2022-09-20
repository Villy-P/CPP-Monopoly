#ifndef PLAYER_H
#define PLAYER_H

#include "board.hpp"

#include <string>
#include <vector>

class Player {
    public:
        bool isMainPlayer;
        std::string name;
        std::string identifier;
        int cash = 1500;
        int plotPosition = 0;
        int bid = 0;
        bool isBidding = true;
        std::vector<Plot> ownedPlots;

        Player(bool isMainPlayer);

        void movePlayer(Board& board, std::vector<Player> computers);
        void buyProperty(Board& board, unsigned char squaresToMove);
        bool reduceMoney(int amount);
};

#endif

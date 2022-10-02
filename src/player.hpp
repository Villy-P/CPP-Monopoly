#ifndef SRC_PLAYER_H_
#define SRC_PLAYER_H_

#include "board.hpp"

#include <string>
#include <vector>

namespace player {
    class Player {
        public:
            bool isMainPlayer;
            std::string name;
            std::string identifier;
            int cash = 1500;
            int plotPosition = 0;
            int bid = 0;
            bool isBidding = true;
            std::vector<plot::Plot> ownedPlots;
            unsigned char getOutOfJailFreeCards = 0;

            Player(bool isMainPlayer);

            void movePlayer(board::Board& board, Player mainPlayer, std::vector<Player> computers);
            void buyProperty(board::Board& board, unsigned char squaresToMove);
            void reduceMoney(int amount, board::Board& board, Player mainPlayer, std::vector<player::Player> computers, bool doesOwe, Player oweTo);
            int moneyCanMake();
    };
}

#endif

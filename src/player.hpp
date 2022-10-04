#ifndef SRC_PLAYER_H_
#define SRC_PLAYER_H_

#include "board.hpp"
#include "plot.hpp"

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
            bool inGame = true;
            std::vector<plot::Plot> ownedPlots;
            unsigned char getOutOfJailFreeCards = 0;

            Player(bool isMainPlayer);

            void movePlayer(board::Board& board, Player mainPlayer, std::vector<Player> computers);
            void reduceMoney(int amount, board::Board& board, Player mainPlayer, std::vector<player::Player> computers, bool doesOwe, Player oweTo);
            int moneyCanMake();
            bool ownsPlot(plot::Plot& plot);
            void computerBankruptcy(board::Board& board, std::vector<Player> computers, Player mainPlayer, bool doesOwe, Player oweTo);
            bool ownsColorSet(std::string color);
            unsigned char ownedRailroads();
            unsigned char ownedUtilities();
            void payRent(plot::Plot nextPlot, board::Board& board, player::Player mainPlayer, std::vector<player::Player> computers, std::vector<unsigned char> dieRoll);
            void payRentOnProperty(plot::Plot nextPlot, board::Board& board, player::Player mainPlayer, std::vector<player::Player> computers, player::Player whoOwns);
            void payRentOnRailroad(plot::Plot nextPlot, board::Board& board, player::Player mainPlayer, std::vector<player::Player> computers, player::Player whoOwns);
            void payRentOnUtility(plot::Plot nextPlot, board::Board& board, player::Player mainPlayer, std::vector<player::Player> computers, player::Player whoOwns, std::vector<unsigned char> dieRoll);
            void buyProperty(plot::Plot nextPlot, board::Board& board, player::Player mainPlayer, std::vector<player::Player> computers);
            void buyPropertyAsMainPlayer(plot::Plot nextPlot, board::Board& board, player::Player mainPlayer, std::vector<player::Player> computers);
            void buyPropertyAsComputer(plot::Plot nextPlot, board::Board& board, player::Player mainPlayer, std::vector<player::Player> computers);
            void buyPropertySegment(plot::Plot nextPlot, board::Board& board, player::Player mainPlayer, std::vector<player::Player> computers);
    };
}

#endif

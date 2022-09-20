#pragma once

#include "plot.hpp"

#include <unordered_set>
#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

std::string center(const std::string s, const int w) {
    std::stringstream ss, spaces;
    int pad = w - s.size();                  // count excess room to pad
    for(int i=0; i<pad/2; ++i)
        spaces << " ";
    ss << spaces.str() << s << spaces.str(); // format with padding
    if(pad>0 && pad%2!=0)                    // if pad odd #, add 1 more space
        ss << " ";
    return ss.str();
}

plot::Plot::Plot(std::unordered_set<std::string> flags, std::unordered_map<std::string, std::string> stringProperties, std::unordered_map<std::string, int> intProperties) {
    this->flags = flags;
    this->stringProperties = stringProperties;
    this->intProperties = intProperties;
}

void plot::Plot::displayTitleCard() {
    unsigned char titleCardLength = 29;
    std::string repeatedDash = std::string(titleCardLength, '-');
    std::string repeatedSpace = std::string(titleCardLength, ' ');
    std::cout << this->stringProperties.at("COLORCODE") << "+" << repeatedDash << "+" << std::endl;
    std::cout << "|" << center("TITLE DEED", titleCardLength) << "|" << std::endl;
    std::cout << "|" << center(this->stringProperties.at("NAME"), titleCardLength) << "|" << std::endl;
    std::cout << "+" << repeatedDash << "+" << std::endl;
    std::cout << "|" << center("RENT $" + std::to_string(this->intProperties.at("RENT")) + ".", titleCardLength) << "|" << std::endl;
    std::cout << "|" << this->getRentWithHouseString(1, "RENTWITHONEHOUSE", titleCardLength) << "|" << std::endl;
    std::cout << "|" << this->getRentWithHouseString(2, "RENTWITHTWOHOUSES", titleCardLength) << "|" << std::endl;
    std::cout << "|" << this->getRentWithHouseString(3, "RENTWITHTHREEHOUSES", titleCardLength) << "|" << std::endl;
    std::cout << "|" << this->getRentWithHouseString(4, "RENTWITHFOURHOUSES", titleCardLength) << "|" << std::endl;
    std::cout << "|" << center("WITH HOTEL $" + std::to_string(this->intProperties.at("RENTWITHHOTEL")) + ".", titleCardLength) << "|" << std::endl;
    std::cout << "|" << repeatedSpace << "|" << std::endl;
    std::cout << "|" << center("MORTGAGE VALUE $" + std::to_string(this->intProperties.at("MORTGAGEVALUE")) + ".", titleCardLength) << "|" << std::endl;
    std::cout << "|" << center("HOUSES COST $" + std::to_string(this->intProperties.at("HOUSESCOST")) + ". EACH", titleCardLength) << "|" << std::endl;
    std::cout << "|";
    std::cout << center("HOTELS, $" + std::to_string(this->intProperties.at("HOTELSCOST")) + ". PLUS " + std::to_string(this->intProperties.at("PLUSHOUSES")), titleCardLength);
    std::cout << "|" << std::endl;
    std::cout << "+" << repeatedDash << "+" << "\033[0m" << std::endl;
}

std::string plot::Plot::getRentWithHouseString(unsigned char number, std::string name, unsigned char titleCardLength) {
    return " WITH " + std::to_string(number) + " HOUSE" + std::string(titleCardLength - 15 - std::to_string(this->intProperties.at(name)).length(), ' ') + "$" + std::to_string(this->intProperties.at(name)) + " ";
}

void plot::Plot::auction(board::Board& board, player::Player player, std::vector<player::Player> computers) {
    std::vector<player::Player> players;
    players.push_back(player);
    for (player::Player computer : computers)
        players.push_back(computer);
    std::cout << "Bidding has started on the property " << this->stringProperties.at("COLORCODE") << this->stringProperties.at("NAME") << functions::ANSI_RESET << std::endl;
    player::Player maxBidder(false);
    while (true) {
        std::cout << "The current bid is $" << std::to_string(maxBidder.bid) << std::endl;
        if (this->playersStillBidding(players))
            break;
        for (player::Player user : players) {
            if (!user.isBidding)
                continue;
            if (user.isMainPlayer) {
                functions::printlnBlue("It's your turn to bid");
                functions::printlnBlue("Your current bid is $" + std::to_string(user.bid));
                int input = functions::readIntInput("Enter a number higher than the current bid minus your current bid (or 0 to stop bidding)>", 0, maxBidder.bid - user.bid);
                if (input == 0) {
                    functions::printlnRed("You have stopped bidding.");
                    user.isBidding = false;
                } else {
                    user.bid += input;

                }
            } else {

            }
        }
    }
}

bool plot::Plot::playersStillBidding(std::vector<player::Player> players) {
    unsigned char playersStillBidding = 0;
    for (player::Player p : players)
        if (p.isBidding)
            playersStillBidding++;
    return playersStillBidding > 1;
}

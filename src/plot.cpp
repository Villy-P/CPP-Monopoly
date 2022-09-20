#ifndef PLOT_CPP
#define PLOT_CPP

#include "plot.hpp"

#include <unordered_set>
#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;


string center(const string s, const int w) {
    stringstream ss, spaces;
    int pad = w - s.size();                  // count excess room to pad
    for(int i=0; i<pad/2; ++i)
        spaces << " ";
    ss << spaces.str() << s << spaces.str(); // format with padding
    if(pad>0 && pad%2!=0)                    // if pad odd #, add 1 more space
        ss << " ";
    return ss.str();
}

Plot::Plot(unordered_set<string> flags, unordered_map<string, string> stringProperties, unordered_map<string, int> intProperties) {
    this->flags = flags;
    this->stringProperties = stringProperties;
    this->intProperties = intProperties;
}

void Plot::displayTitleCard() {
    unsigned char titleCardLength = 29;
    string repeatedDash = string(titleCardLength, '-');
    string repeatedSpace = string(titleCardLength, ' ');
    cout << this->stringProperties.at("COLORCODE") << "+" << repeatedDash << "+" << endl;
    cout << "|" << center("TITLE DEED", titleCardLength) << "|" << endl;
    cout << "|" << center(this->stringProperties.at("NAME"), titleCardLength) << "|" << endl;
    cout << "+" << repeatedDash << "+" << endl;
    cout << "|" << center("RENT $" + to_string(this->intProperties.at("RENT")) + ".", titleCardLength) << "|" << endl;
    cout << "|" << this->getRentWithHouseString(1, "RENTWITHONEHOUSE", titleCardLength) << "|" << endl;
    cout << "|" << this->getRentWithHouseString(2, "RENTWITHTWOHOUSES", titleCardLength) << "|" << endl;
    cout << "|" << this->getRentWithHouseString(3, "RENTWITHTHREEHOUSES", titleCardLength) << "|" << endl;
    cout << "|" << this->getRentWithHouseString(4, "RENTWITHFOURHOUSES", titleCardLength) << "|" << endl;
    cout << "|" << center("WITH HOTEL $" + to_string(this->intProperties.at("RENTWITHHOTEL")) + ".", titleCardLength) << "|" << endl;
    cout << "|" << repeatedSpace << "|" << endl;
    cout << "|" << center("MORTGAGE VALUE $" + to_string(this->intProperties.at("MORTGAGEVALUE")) + ".", titleCardLength) << "|" << endl;
    cout << "|" << center("HOUSES COST $" + to_string(this->intProperties.at("HOUSESCOST")) + ". EACH", titleCardLength) << "|" << endl;
    cout << "|";
    cout << center("HOTELS, $" + to_string(this->intProperties.at("HOTELSCOST")) + ". PLUS " + to_string(this->intProperties.at("PLUSHOUSES")), titleCardLength);
    cout << "|" << endl;
    cout << "+" << repeatedDash << "+" << "\033[0m" << endl;
}

string Plot::getRentWithHouseString(unsigned char number, string name, unsigned char titleCardLength) {
    return " WITH " + to_string(number) + " HOUSE" + string(titleCardLength - 15 - to_string(this->intProperties.at(name)).length(), ' ') + "$" + to_string(this->intProperties.at(name)) + " ";
}

void Plot::auction(Board& board, Player player, std::vector<Player> computers) {
    vector<Player> players;
    players.push_back(player);
    for (Player computer : computers)
        players.push_back(computer);
    cout << "Bidding has started on the property " << this->stringProperties.at("COLORCODE") << this->stringProperties.at("NAME") << functions::ANSI_RESET << endl;
    Player maxBidder(false);
    while (true) {
        cout << "The current bid is $" << to_string(maxBidder.bid) << endl;
        if (this->playersStillBidding(players))
            break;
        for (Player user : players) {
            if (!user.isBidding)
                continue;
            if (user.isMainPlayer) {
                functions::printlnBlue("It's your turn to bid");
                functions::printlnBlue("Your current bid is $" + to_string(user.bid));
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

bool Plot::playersStillBidding(std::vector<Player> players) {
    unsigned char playersStillBidding = 0;
    for (Player p : players)
        if (p.isBidding)
            playersStillBidding++;
    return playersStillBidding > 1;
}

#endif

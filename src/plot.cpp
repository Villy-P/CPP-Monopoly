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
    player.bid = 0;
    player.isBidding = true;
    for (Player& computer : computers) {
        computer.bid = 0;
        computer.isBidding = true;
    }
    functions::printlnBlue("Auctioning has begun on the property: " + this->stringProperties.at("COLORCODE") + this->stringProperties.at("NAME") + "\033[0m");
    functions::printlnBlue("The bid will begin at $1");
    int maxBid = 1;
    Player maxBidder = player;
    while (true) {
        if (player.isBidding) {
            functions::printlnRed("It's your turn to bid");
            functions::printlnBlue("The max bid is " + to_string(maxBid));
            functions::printlnCyan("You currently have $" + to_string(player.cash) + " and are bidding $" + to_string(playerBid));
            functions::printlnBlue("Enter 0 if you don't want to bid.");
            int input = functions::readIntInput(
                "Enter a number that is 0 and greater that the max bid and less then the amount of cash you have minus the amount you are currently bidding>", 
                maxBid, 
                player.cash - playerBid
            );
            if (input == 0) {
                player.isBidding = false;
                functions::printlnRed("You have stopped bidding.");
            } else {
                player.bid += input;
                functions::printlnMagenta("Your bid has risen to " + to_string(playerBid));
            }
        }
    }
}
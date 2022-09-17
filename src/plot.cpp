#include "plot.h"

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
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
    cout << this->stringProperties.at("COLORCODE") << "+" << repeatedDash << "+" << endl;
    cout << "|" << center("TITLE DEED", titleCardLength) << "|" << endl;
    cout << "|" << center(this->stringProperties.at("NAME"), titleCardLength) << "|" << endl;
}
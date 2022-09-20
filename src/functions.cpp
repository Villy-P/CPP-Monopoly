#ifndef FUNCTIONS_CPP
#define FUNCTIONS_CPP

#include "functions.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>

using namespace std;

void functions::clear() {
    cout << "\033[2J" << endl;
}

string functions::center(const string s, const int w) {
    stringstream ss, spaces;
    int pad = w - s.size();                  // count excess room to pad
    for(int i=0; i<pad/2; ++i)
        spaces << " ";
    ss << spaces.str() << s << spaces.str(); // format with padding
    if(pad>0 && pad%2!=0)                    // if pad odd #, add 1 more space
        ss << " ";
    return ss.str();
}

void functions::printlnRed(string str)     {cout << functions::ANSI_RED     << str << functions::ANSI_RESET << endl;}
void functions::printlnBlue(string str)    {cout << functions::ANSI_BLUE    << str << functions::ANSI_RESET << endl;}
void functions::printlnGreen(string str)   {cout << functions::ANSI_GREEN   << str << functions::ANSI_RESET << endl;}
void functions::printlnYellow(string str)  {cout << functions::ANSI_YELLOW  << str << functions::ANSI_RESET << endl;}
void functions::printlnMagenta(string str) {cout << functions::ANSI_MAGENTA << str << functions::ANSI_RESET << endl;}
void functions::printlnCyan(string str)    {cout << functions::ANSI_CYAN    << str << functions::ANSI_RESET << endl;}

bool functions::setContains(std::unordered_set<std::string> set, std::string str) {
    return set.count(str);
}

std::vector<std::string> functions::split(const std::string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<std::string> elems;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
        // elems.push_back(std::move(item)); // if C++11 (based on comment from @mchiasson)
    }
    return elems;
}

string functions::readStringInput(string prompt) {
    string returnString;
    cout << prompt;
    getline(cin, returnString);
    return returnString;
}

string functions::readCharInput(string prompt) {
    string returnString;
    cout << prompt;
    getline(cin, returnString);
    while (returnString.length() != 1) {
        cout << "Enter a character>";
        getline(cin, returnString);
    }
    return returnString;
}

bool functions::tryParse(string &input, int &output) {
    try {
        output = stoi(input);
    } catch (invalid_argument) {
        return false;
    }
    return true;
}

int functions::readIntInput(string prompt, int min, int max) {
    string input;
    cout << prompt;
    int x;
    getline(cin, input);
    while (!tryParse(input, x) || x < min || x > max) {
        cout << "Enter a number between " << min << " & " << max << ">";
        getline(cin, input);
    }
    return x;
}

#endif

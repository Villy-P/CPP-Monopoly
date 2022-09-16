#include "functions.h"

#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

void functions::clear() {
    cout << "\033[2J" << endl;
}

void functions::printlnRed(string str)     {cout << functions::ANSI_RED     << str << functions::ANSI_RESET << endl;}
void functions::printlnBlue(string str)    {cout << functions::ANSI_BLUE    << str << functions::ANSI_RESET << endl;}
void functions::printlnGreen(string str)   {cout << functions::ANSI_GREEN   << str << functions::ANSI_RESET << endl;}
void functions::printlnYellow(string str)  {cout << functions::ANSI_YELLOW  << str << functions::ANSI_RESET << endl;}
void functions::printlnMagenta(string str) {cout << functions::ANSI_MAGENTA << str << functions::ANSI_RESET << endl;}
void functions::printlnCyan(string str)    {cout << functions::ANSI_CYAN    << str << functions::ANSI_RESET << endl;}

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

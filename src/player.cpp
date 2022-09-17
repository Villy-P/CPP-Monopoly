#include "player.h"

#include <vector>
#include <iostream>

using namespace std;

namespace functions {};

Player::Player(bool isMainPlayer) {
    this->isMainPlayer = isMainPlayer;
}

void Player::movePlayer(Board& board) {
    functions::clear();
    vector<unsigned char> dieRoll = board.rollDice();
    int squaresToMove = 1;
    string currentSquareColor =  board.getStringProperty(this->plotPosition, "COLORCODE");
    string currentSquareName = board.getStringProperty(this->plotPosition, "NAME");
    string nextSquareColor = board.getStringProperty((this->plotPosition + squaresToMove), "COLORCODE");
    string nextSquareName = board.getStringProperty(this->plotPosition + squaresToMove, "NAME");
    functions::printlnBlue(this->name + " rolled:");
    cout << "+---+ +---+" << endl;
    cout << "| " << to_string(dieRoll[0]) << " | | " << to_string(dieRoll[1]) << " |" << endl;
    cout << "+---+ +---+" << endl;
    cout << this->name << " moved " << to_string(squaresToMove) << " spaces from" << endl;
    cout << currentSquareColor << "+" << string(currentSquareName.length() + 2, '-') << "+" << endl;
    cout << "| " << currentSquareName << " |" << endl;
    cout << "+" << string(currentSquareName.length() + 2, '-') << "+" << functions::ANSI_RESET << endl;
    cout << "To" << endl;
    cout << nextSquareColor << "+" << string(nextSquareName.length() + 2, '-') << "+" << endl;
    cout << "| " << nextSquareName << " |" << endl;
    cout << "+" << string(nextSquareName.length() + 2, '-') << "+" << functions::ANSI_RESET << endl;
    for (unsigned char i = this->plotPosition; i < this->plotPosition + squaresToMove; i++) {
        Plot plot = board.plots[i];
        if (this->plotPosition != 0 && plot.flags.find("GOSQUARE") != plot.flags.end()) {
            cout << this->name << " passes by GO. +$200";
            this->cash += 200;
        }
    }
    cout << this->name << " landed on " << nextSquareColor << nextSquareName << functions::ANSI_RESET << endl;
}

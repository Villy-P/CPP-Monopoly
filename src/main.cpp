#include "player.cpp"

#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <thread>

using namespace std;

string colorMenu() {
    cout << "Next, pick a color!" << endl;
    functions::printlnRed("1: Red");
    functions::printlnGreen("2: Green");
    functions::printlnYellow("3: Yellow");
    functions::printlnBlue("4: Blue");
    functions::printlnMagenta("5: Magenta");
    functions::printlnCyan("6: Cyan");
    switch (functions::readIntInput(">", 1, 6)) {
        case 1: return functions::ANSI_RED;
        case 2: return functions::ANSI_GREEN;
        case 3: return functions::ANSI_YELLOW;
        case 4: return functions::ANSI_BLUE;
        case 5: return functions::ANSI_MAGENTA;
        case 6: return functions::ANSI_CYAN;
    }
    return functions::ANSI_WHITE;
}

int main() {
    functions::clear();
    Board board;
    board.createPlots();
    Player player(true);
    functions::printlnBlue("Welcome to Monopoly!");
    functions::printlnGreen("What is your name?");
    player.name = functions::readStringInput(">");
    functions::clear();
    functions::printlnMagenta("How many computers do you want to play against? (1-7)");
    unsigned char computerNumber = functions::readIntInput(">", 1, 7);
    vector<Player> computers;
    vector<string> computerIdentifiers{"!", "@", "#", "$", "%", "^", "&", "*"};
    for (unsigned char i = 0; i < computerNumber; i++) {
        computers.push_back(Player(false));
        computers[i].name = "Computer #" + to_string(i);
    }
    functions::clear();
    functions::printlnCyan("First, pick an identifier! (Must be single character)");
    player.identifier = functions::readCharInput(">");
    computerIdentifiers.erase(std::remove(computerIdentifiers.begin(), computerIdentifiers.end(), player.identifier), computerIdentifiers.end());
    functions::clear();
    string identifierColor = colorMenu();
    player.identifier = identifierColor + player.identifier + functions::ANSI_RESET;
    functions::clear();
    functions::printlnBlue("You will be identified as " + player.identifier);
    functions::printlnGreen("Here are your opponents:");
    for (unsigned char i = 0; i < computerNumber; i++) {
        computers[i].identifier = computerIdentifiers[i];
        functions::printlnRed(computers[i].name + " as " + computers[i].identifier);
    }
    vector<unsigned char> playerDice = board.rollDice();
    unsigned char firstMoverCount = playerDice[0] + playerDice[1];
    Player firstMover = player;
    for (unsigned char i = 0; i < computerNumber; i++) {
        vector<unsigned char> dice = board.rollDice();
        if (dice[0] + dice[1] > firstMoverCount) {
            firstMoverCount = dice[0] + dice[1];
            firstMover = computers[i];
        }
    }
    functions::printlnGreen(firstMover.name + " will be moving first!");
    functions::readStringInput("");
    firstMover.movePlayer(board, computers);
}

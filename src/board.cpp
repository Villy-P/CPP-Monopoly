#include "board.h"
#include "plot.cpp"
#include "functions.cpp"

#include <vector>
#include <string>
#include <random>
#include <fstream>

using namespace std;

vector<unsigned char> Board::rollDice() {
    random_device random;
    mt19937 generate(random());
    uniform_int_distribution<> distr(1, 6);
    unsigned char die1 = distr(generate);
    unsigned char die2 = distr(generate);
    vector vect{die1, die2};
    return vect;
}

void Board::createPlots() {
    ifstream plotData("C:\\Users\\lilyp\\OneDrive\\Documents\\Projects\\C++\\CPP-Monopoly\\src\\plotData.txt");
    Plot plot({}, {}, {});
    if (plotData.is_open()) {
        while (plotData.good()) {
            string next;
            getline(plotData, next); // Get [FLAGS]
            getline(plotData, next); // Get first flag (OR STRING_PROPERTIES)
            while (next != "[STRING_PROPERTIES]") {
                plot.flags.insert(next);
                getline(plotData, next);
            }
            getline(plotData, next);
            while (next != "[INT_PROPERTIES]") {
                vector<string> split = functions::split(next, '=');
                if (split[0] == "COLORCODE") {
                    if (split[1] == "WHITE")
                        split[1] = functions::ANSI_WHITE;
                    else if (split[1] == "BROWN")
                        split[1] = functions::ANSI_BROWN;
                    else if (split[1] == "DARK_CYAN")
                        split[1] = functions::ANSI_DARK_CYAN;
                    else if (split[1] == "CYAN")
                        split[1] = functions::ANSI_CYAN;
                }
                plot.stringProperties.insert({{split[0], split[1]}});
                getline(plotData, next);
            }
            getline(plotData, next);
            while (next != "-NEWPLOT-") {
                vector<string> split = functions::split(next, '=');
                plot.intProperties.insert({{split[0], stoi(split[1])}});
                getline(plotData, next);
            }
            getline(plotData, next);
            // cout << next << endl;
            this->plots.push_back(plot);
            plot = Plot{{}, {}, {}};
        }
    }
}

string Board::getStringProperty(int index, string propertyName) {
    int trueIndex;
    if (index > this->plots.size())
        trueIndex = index - this->plots.size();
    else
        trueIndex = index;
    return this->plots.at(trueIndex).stringProperties.at(propertyName);
}

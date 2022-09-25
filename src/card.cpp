#ifndef CARD_CPP
#define CARD_CPP

#include "card.hpp"

#include "functions.cpp"

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <fstream>

void card::Card::createCards() {
    std::ifstream cardData("cardData.txt");
    card::Card card(true, "", {}, {});
    if (cardData.is_open()) {
        while (cardData.good()) {
            std::string next;
            std::getline(cardData, next); // Get [FLAGS]
            std::getline(cardData, next); // Get first flag (OR STRING_PROPERTIES)
            while (next != "[STRING_PROPERTIES]") {
                card.flags.insert(next);
                std::getline(cardData, next);
            }
            std::getline(cardData, next);
            while (next != "[INT_PROPERTIES]") {
                std::vector<std::string> split = functions::split(next, '=');
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
                std::getline(cardData, next);
            }
            std::getline(cardData, next);
            while (next != "-NEWPLOT-") {
                std::vector<std::string> split = functions::split(next, '=');
                plot.intProperties.insert({{split[0], stoi(split[1])}});
                getline(cardData, next);
            }
            this->plots.push_back(plot);
            plot = plot::Plot{{}, {}, {}};
        }
    }
}

card::Card::Card(bool isChance, std::string description, std::unordered_set<std::string> flags, std::unordered_map<std::string, std::string> properties) {
    this->isChance = isChance;
    this->description = description;
    this->flags = flags;
    this->properties = properties;
}

#endif
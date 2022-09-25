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
            std::getline(cardData, next);
            card.isChance = functions::split(next, '=')[1] == "CHANCE";
            std::getline(cardData, next);
            card.description = functions::split(next, '=')[1];
            std::getline(cardData, next); // Get [FLAGS]
            std::getline(cardData, next); // Get first flag (OR PROPERTIES)
            while (next != "[PROPERTIES]") {
                card.flags.insert(next);
                std::getline(cardData, next);
            }
            std::getline(cardData, next);
            while (next != "-NEWCARD-") {
                std::vector<std::string> split = functions::split(next, '=');
                card.properties.insert({{split[0], split[1]}});
                getline(cardData, next);
            }
            card.isChance ? card::Card::chanceCards.push_back(card) : card::Card::chestCards.push_back(card);
            card = card::Card(true, "", {}, {});
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
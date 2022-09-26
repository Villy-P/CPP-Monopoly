#ifndef CARD_CPP
#define CARD_CPP

#include "card.hpp"

#include "functions.cpp"

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <random>

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

void card::Card::shuffleDeck() {
    std::default_random_engine rng = std::default_random_engine {};
    std::shuffle(std::begin(card::Card::chanceCards), std::end(card::Card::chanceCards), rng);
    std::shuffle(std::begin(card::Card::chestCards), std::end(card::Card::chestCards), rng);
}

void card::Card::drawChanceCard() {
    card::Card card = *functions::select_randomly(card::Card::chanceCards.begin(), card::Card::chestCards.end());
    std::vector<std::string> displaySentence = functions::split(card.description, ' ');
    unsigned char lengthOfEach = displaySentence.size() / 5;
    std::cout << "+----------------------------+" << std::endl;
    std::cout << "|            OOOOOO          |" << std::endl;
    std::cout << "|            O    O          |" << std::endl;
    std::cout << "|              OOOO          |" << std::endl;
    std::cout << "|              O             |" << std::endl;
    std::cout << "|              .             |" << std::endl;
    std::cout << "+----------------------------+" << std::endl;
    functions::readStringInput("");
    std::cout << "+----------------------------+" << std::endl;
    for (unsigned char i = 0; i < 5; i++) {
        std::string str = "";
        for (unsigned char j = 0; j < lengthOfEach; j++) {
            str += displaySentence[j];
        }
        std::cout << "+" << functions::center(str, 28) << "+" << std::endl;
    }
    std::cout << "+----------------------------+" << std::endl;
}

card::Card::Card(bool isChance, std::string description, std::unordered_set<std::string> flags, std::unordered_map<std::string, std::string> properties) {
    this->isChance = isChance;
    this->description = description;
    this->flags = flags;
    this->properties = properties;
}

#endif
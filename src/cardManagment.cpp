#ifndef CARD_MANAGMENT_CPP
#define CARD_MANAGMENT_CPP

#include "cardManagment.hpp"
#include "functions.cpp"

#include <algorithm>
#include <random>
#include <iostream>
#include <fstream>

void card_managment::CardManagment::shuffleDecks() {
    std::default_random_engine rng = std::default_random_engine {};
    std::shuffle(this->chanceCards.begin(), this->chanceCards.end(), rng);
    std::shuffle(std::begin(this->chestCards), std::end(this->chestCards), rng);
    for (auto c : this->chanceCards)
        std::cout << c.description << std::endl;
}

void card_managment::CardManagment::createCards() {
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
            card.isChance ? this->chanceCards.push_back(card) : this->chestCards.push_back(card);
                card = card::Card(true, "", {}, {});
        }
    }
}

void card_managment::CardManagment::drawChanceCard() {
    card::Card card = this->chanceCards[0];
    std::vector<std::string> displaySentence = functions::split(card.description, ' ');
    unsigned char lengthOfEach = displaySentence.size() / 5;
    unsigned char extraLetters = displaySentence.size() % 5;
    std::cout << "+------------------------------+" << std::endl;
    std::cout << "|            OOOOOO            |" << std::endl;
    std::cout << "|            O    O            |" << std::endl;
    std::cout << "|              OOOO            |" << std::endl;
    std::cout << "|              O               |" << std::endl;
    std::cout << "|              .               |" << std::endl;
    std::cout << "+------------------------------+" << std::endl;
    functions::readStringInput("");
    std::cout << "+----------------------------+" << std::endl;
    char count = -1;
    for (unsigned char i = 0; i < 5; i++) {
        count++;
        std::string str = "";
        for (unsigned char j = 0; j < lengthOfEach; j++) {
            str += displaySentence[j + count] + " ";
            if (extraLetters > 0) {
                count++;
                str += displaySentence[j + count] += " ";
                extraLetters--;
            }
        }
        str.pop_back();
        std::cout << "|" << functions::center(str, 28) << "|" << std::endl;
        count++;
    }
    std::cout << "+----------------------------+" << std::endl;
    this->chanceCards.erase(this->chanceCards.begin());
    this->chanceCards.push_back(card);
}

#endif
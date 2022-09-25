#ifndef CARD_CPP
#define CARD_CPP

#include "card.hpp"

#include <string>
#include <unordered_set>
#include <unordered_map>

void card::Card::createCards() {

}

card::Card::Card(bool isChance, std::string name, std::string description, std::unordered_set<std::string> flags, std::unordered_map<std::string, std::string> properties) {
    this->isChance = isChance;
    this->name = name;
    this->description = description;
    this->flags = flags;
    this->properties = properties;
}

#endif
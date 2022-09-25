#ifndef CARD_CPP
#define CARD_CPP

#include "card.hpp"

#include <string>
#include <unordered_set>
#include <unordered_map>

card::Card::Card(bool isChance, std::string name, std::string description, std::unordered_set<std::string> flags, std::unordered_map<std::string, std::string> stringProperties) {
    this->isChance = isChance;
    this->name = name;
    this->description = description;
    this->flags = flags;
    this->stringProperties = stringProperties;
}

#endif
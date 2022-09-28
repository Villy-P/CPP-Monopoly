#ifndef CARD_MANAGMENT_HPP
#define CARD_MANAGMENT_HPP

#include <vector>

#include "card.hpp"

namespace card_managment {
    class CardManagment {
        public: 
            std::vector<card::Card> chanceCards;
            std::vector<card::Card> chestCards;

            void shuffleDecks();
            void drawChanceCard();
            void createCards();
    };
}

#endif
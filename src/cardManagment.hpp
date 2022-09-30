#ifndef CARD_MANAGMENT_HPP
#define CARD_MANAGMENT_HPP

#include <deque>

#include "card.hpp"

namespace card_managment {
    class CardManagment {
        public: 
            std::deque<card::Card> chanceCards;
            std::deque<card::Card> chestCards;

            void shuffleDecks();
            void drawChanceCard();
            void drawChestCard();
            void createCards();
    };
}

#endif
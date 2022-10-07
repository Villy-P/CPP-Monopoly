#ifndef CARD_MANAGMENT_HPP
#define CARD_MANAGMENT_HPP

#include <deque>

#include "card.hpp"
#include "board.hpp"

namespace card_managment {
    class CardManagment {
        public: 
            std::deque<card::Card> chanceCards;
            std::deque<card::Card> chestCards;

            void shuffleDecks();
            void drawChanceCard();
            void drawChestCard();
            void createCards();

            void redeemCard(board::Board& board, player::Player& mainPlayer, std::vector<player::Player>& computers, player::Player& player, card::Card& card);
    };
}

#endif
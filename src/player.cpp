#include "player.h"

#include <vector>

using namespace std;

namespace functions {};

Player::Player(bool isMainPlayer) {
    this->isMainPlayer = isMainPlayer;
}

void Player::movePlayer(Board& board) {
    functions::clear();
}

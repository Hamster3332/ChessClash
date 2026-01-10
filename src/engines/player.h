#ifndef PLAYER_H
#define PLAYER_H

#include "../core/board.h"

class Player {
public:
    Player(Board& board);
    void startTurn(Move LastTurn);
    bool calculate(Move playerMove);
    void promotionResult(unsigned char piece);
    bool isReady();
};

#endif

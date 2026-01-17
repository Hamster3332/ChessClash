#ifndef ACTIVE_PLAYER_INTERFACE_H
#define ACTIVE_PLAYER_INTERFACE_H

#include "chessVector.h"

class activePlayerInterface {
public:
    virtual void activeMove(Move currentMove) {}
    virtual void promotionResult(unsigned char piece) {}
};
#endif

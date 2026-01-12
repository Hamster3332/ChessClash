#ifndef ACTIVE_PLAYER_INTERFACE_H
#define ACTIVE_PLAYER_INTERFACE_H

#include "../core/chessVector.h"

class ActivePlayerInterface {
public:
    virtual void activeMove(Move currentMove) {}
    virtual void promotionResult(unsigned char piece) {}
    virtual void startTurn(Move LastTurn){}
    virtual Move calculate(){return {{0,0}, {0,0}};}
    virtual bool isReady(){return false;}
};
#endif

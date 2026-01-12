#ifndef PLAYER_INTERFACE_H
#define PLAYER_INTERFACE_H

#include "../core/chessVector.h"

class PlayerInterface {
public:
    virtual void startTurn(Move LastTurn){}
    virtual Move calculate(){return {{0,0}, {0,0}};}
    virtual bool isReady(){return false;}
};
#endif

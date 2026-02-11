#ifndef PLAYER_INTERFACE_H
#define PLAYER_INTERFACE_H

#include "../core/chessVector.h"
#include "board.h"
#include <vector>

class PlayerInterface {
public:
    virtual ~PlayerInterface() = default;

    virtual void startBot(enPlayers _color){}
    virtual bool isReady() {return false;}
    virtual void startTurn(Move LastTurn){}
    virtual Move calculate() {return {{0,0}, {0,0}};}
    virtual unsigned char getPromotion() {return 'q';}
    virtual Move fallback(std::vector<Move> &moves){return {{-1, -1}, {-1, -1}};}

};
#endif

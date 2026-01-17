#ifndef PLAYER_H
#define PLAYER_H

#include <board.h>
#include <renderBoard.h>
#include "playerInterface.cpp"
#include "activePlayerInterface.cpp"


class Player : public activePlayerInterface, public PlayerInterface {
public:
    Player(Board& board, RenderBoard& renderer);
    void startBot(enPlayers _color) override;
    bool isReady() override;
    void startTurn(Move LastTurn) override;
    Move calculate() override;
    unsigned char getPromotion() override;
    Move fallback(std::vector<Move> &moves) override;
    
    void activeMove(Move currentMove) override;
    void promotionResult(unsigned char piece) override;

    Board* curBoard;
    RenderBoard* curRenderer;
    Move playerMove = {{-1, -1}, {-1, -1}};
    unsigned char promotionPiece = '.';
};

#endif
